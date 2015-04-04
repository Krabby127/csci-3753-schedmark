/* A simple scheduling and running program used for benchmarking.
 *
 * Copyright (C) 2015  Michael R. Shannon
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */




#include <stdlib.h>
#include <stdio.h>

// needed for "getopt"
#include <unistd.h>

// needed for "getuid"
#include <unistd.h>
#include <sys/types.h>

// needed for "sched_setscheduler"
#include <sched.h>

// needed "getpwnam"
#include <sys/types.h>
#include <pwd.h>

// needed for "setuid" and "setgid"
#include <unistd.h>

// needed for "execv"
#include <unistd.h>

// needed for "log10", "ceil", and "pow"
#include <math.h>

// needed for "wait"
#include <sys/types.h>
#include <sys/wait.h>




#define SCHEDULER_CFS 0
#define SCHEDULER_FCFS 1
#define SCHEDULER_RR 2


struct app_s{
    int pid_location;
    int pid_index;
    unsigned int processes;
    int scheduler;
    char *username;
    char *command;
    int argc;
    char **argv;
};




// display program help
void help(char *program)
{
fprintf(stdout,
"NAME\n"
"       schedule - runs a program multiple times with a given scheduler\n"
"\n"
"SYNOPSIS\n"
"       %s [-h] [-p] [-a] [-n processes] [-s scheduler] command ... args\n"
"\n"
"DESCRIPTION\n"
"       Takes the path to an executable and forks a given number of\n"
"       processes to run the executable simultaneously using a given\n"
"       scheduler\n"
"\n"
"OPTIONS\n"
"       -h      Display this message\n"
"\n"
"       -p      Pass the pid of the new process as the first argument.  If\n"
"               given after -a this will override"
"\n"
"       -a      Pass the pid of the new process as the last argument.  If\n"
"               given after -p this will override."
"\n"
"       -n processes\n"
"               Sets the number of times to run the program."
"\n"
"       -s scheduler\n"
"               Sets the scheduler used, the values are given below:\n"
"\n"
"                   0 = completely fair scheduler (CFS) (default)\n"
"                   1 = real time first come first serve (FCFS)\n"
"                   2 = real time round robin"
"\n"
"       -u username\n"
"               Sets the username to drop into when run as root.\n"
"\n"
"       Any arguments given after the command (the program to execute) are\n"
"       passed on to the child program.\n"
"\n"
"updated: Tuesday, March 31, 2015\n"
,program);
}




// initialize application
int app_init(struct app_s *app, int argc, char *argv[]){


    // initialize defaults
    app->pid_location = 0; // do not use pid
    app->pid_index = -1; // no pid
    app->processes = 1; // one process
    app->scheduler = 0; // CFS
    app->username = NULL; // to check against
    app->command = NULL; // to check against
    app->argc = 0;
    app->argv = NULL;
    

    // parse flagged arguments
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "hpan:s:u:")) != -1){

        switch (c){
            case 'h':
                help(argv[0]);
                return 0;
                break;
            case 'p':
                app->pid_location = -1;
                break;
            case 'a':
                app->pid_location = +1;
                break;
            case 'n':
                if (sscanf(optarg, "%d", &(app->processes)) != 1){
                    fprintf(stderr,
                            "Expected unsigned integer argument after -n"
                            " flag.\n");
                    return -1;
                }
                break;
            case 's':
                if (sscanf(optarg, "%d", &(app->scheduler)) != 1){
                    fprintf(stderr,
                            "Expected integer argument after -s flag.\n");
                    return -1;
                }
                if (app->scheduler > 2 || app->scheduler < 0){
                    fprintf(stderr, "Expected 0, 1, or 2.\n");
                    return -1;
                }
                break;
            case 'u':
                app->username = optarg;
        }
    }

    // get command
    if (optind >= argc){
        fprintf(stderr, "Command not given.\n");
        return -1;
    }
    app->command = argv[optind++];

    // pass along extra arguments
    app->argc = argc - optind + 1 + 1;
    if (app->pid_location){
        app->argc++;
    }
    app->argv = malloc(app->argc*sizeof(char*));
    size_t i = 0;
    app->argv[i++] = app->command;
    if (app->pid_location == -1){
        app->pid_index = i;
        app->argv[i++] = NULL;
    }
    for (;optind < argc; ++optind, ++i){
        app->argv[i] = argv[optind];
    }
    if (app->pid_location == +1){
        app->pid_index = i;
        app->argv[i++] = NULL;
    }

    // check compatibility of username and scheduler
    if (app->scheduler == 1 || app->scheduler == 2){
        if (app->username == NULL){
            fprintf(stderr, "Must supply username to use realtime"
                            " schedulers.\n");
            return -1;
        }
    }
    app->argv[app->argc - 1] = NULL;

    // make sure a username is given if run as root
    if (getuid() == 0 && app->username == NULL){
        fprintf(stderr, "Must supply a username to drop into if run as"
                        " root.\n");
        return -1;
    }

    // make sure running as root if using realtime scheduler
    if ((app->scheduler == 1 || app->scheduler == 2) && getuid() != 0){
        fprintf(stderr, "To use a realtime scheduler this program must\n"
                        " be run as root.\n");
        return -1;
    }

    return 0;
}




// set scheduler
int set_scheduler(int scheduler, char *username)
{
    // convert to SCHED format
    int policy;
    switch (scheduler){
        case SCHEDULER_CFS:
            policy = SCHED_OTHER;
            break;
        case SCHEDULER_FCFS:
            policy = SCHED_FIFO;
            break;
        case SCHEDULER_RR:
            policy = SCHED_RR;
            break;
        default:
            fprintf(stderr, "Unknown scheduler %d\n", scheduler);
            return -1;
            break;
    } // other not needed because already checked

    // set priority
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(policy);

    // set the scheduler
    sched_setscheduler(0, policy, &param);

    // if root drop to given user
    if (getuid() == 0){

        // get user id
        struct passwd *user = getpwnam(username);
        if (user == NULL){
            return -1;
        }

        // drop to user
        setuid(user->pw_uid);
    }

    return 0;
}




int main(int argc, char* argv[])
{

    // initialize application structure
    int err = 0;
    struct app_s app;
    err = app_init(&app, argc, argv);
    if (err){
        if (err < 0){
            fprintf(stderr, "Failed to initialize application.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    // set scheduler
    err = set_scheduler(app.scheduler, app.username);
    if (err){
        fprintf(stderr, "failed to set scheduler to %d\n", app.scheduler);
        return EXIT_FAILURE;
    }

    // initialize pid string
    size_t pid_str_len = (size_t)ceil(log10(pow(2,sizeof(pid_t)))) + 2;
    char pid_str[pid_str_len];
    pid_str[pid_str_len - 1] = '\0';

    // start processes
    for (size_t i = 0; i < app.processes; ++i){

        // fork and exec the new program
        pid_t pid = fork();
        if (pid == 0){ // child
            if (app.pid_location){
                sprintf(pid_str, "%d", getpid());
                app.argv[app.pid_index] = pid_str;
            }
            execv(app.command, app.argv);
        }
    }

    // wait for children to terminate
    while (wait(NULL) > 0);

    return EXIT_SUCCESS;
}
