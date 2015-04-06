% An Analysis of Linux Schedulers
% Michael R. Shannon, Michael N. Eller
% Saturday, April 04, 2015





Introduction
============

Linux contains four CPU schedulers, CFS (completly fair scheduler), FCFS (first
come first serve), RR (round robin), and EDF (earliest deadline first).  In this
report the first three will be analyzed by using simple benchmarking.  All but
the CFS scheduler are realtime schedulers.  

This benchmarking will be done by examining CPU bound, IO bound, and mixed (CPU
and IO) processes with the system under load.  Times, percentages, and context
switches will be plotted and the results explained.  This was made possible by
specially written prime solving code (written by the author of this report) and
IO bound code from the lab handout [@handout].




Methods
=======

Process Types
-------------

In order to examine the Linux schedulers with processes that have heavy CPU
usage a prime computation library (`primes.c`) and program (`prime.c`) were
developed.  It was based on the algorithm found on the [Cython
Tutorial](http://docs.cython.org/src/tutorial/cython_tutorial.html) which is
essentially a search through every integer after 2 checking to see if it is
divisible by the previous primes.

To test the schedulers behavior with a heavy load of IO the `rw` program (which
came with the assignment handout [@handout]) was used.  This program
copies blocks from one file to make a new file.  In particular, it does this
synchronously, avoiding system buffers that may interfere with the results of
the testing.  It is also capable of generating a unique filename based on its
PID (process id) which became useful in the benchmarking process.  It is
important to note that the default blocksize is 1024 bytes and the total amount
of data transferred is 100 blocks by default.

The final mode of usage tested was a mix between IO and CPU heavy processes.  To
accomplish this the `primes` library and `prime` program were modified to allow
writing the output to a file, 512 bytes at a time, using the same synchronous IO
used by `rw`.  Initially this was done in 4 byte (one prime) increments but was
changed due to being excessively IO driven.


Scheduling
----------

The above processes covered the three types of processes that needed to be
analyzed, however, these needed to be run under different schedulers and system
loads.  To accomplish this the `schedule.c` program was written.  This program
does two things.  It first sets the scheduler to either the CFS scheduler, the
soft realtime RR scheduler, or the soft realtime FCFS scheduler (known internally
to Linux as FIFO).  The second role it performs is to fork off a given number of
processes (which inherit the selected scheduler) and runs the program given to
`schedule` in each process.  This is done through `fork`, `exec`, and `wait`.
`schedule` can also optionally pass the PID of the child as an argument to the
command it execs.

The specifics of the settings used to call `prime` and `schedule` was to
generate the first 10,000 primes and to use 10, 100, or 1000 processes depending
on whether the benchmark was to test a light, medium, or heavy system load.
Therefore, it was only necessary to run each possible combination of load,
process, and scheduler, 27 in all.  This was done with the `benchmark` Bourne
shell script.


Benchmarking
------------

The `benchmark` script calls each of the aforementioned combinations and
measures the times, CPU usage, and context switches with the `time` program
located at `/usr/bin/time`.  It directs the output of the `time` program to
individual files (found in Appendix A under "Raw Data").  `benchmark` takes an
optional argument that is the number of times to run the benchmarks, subsequent
runs of these benchmarks are appended to, rather than overwriting these files.
In the case of this report 20 rounds of benchmarking where used.

This however, gives multiple benchmark rounds and does not report the wait time.
To deal with the first another shell script `analyze` is used which makes heavy
use of `awk` to compute the average values place them into files specific to the
scheduler and measurement such as `cfs-time.dat`, `rr-percent.dat`, and
`fcfs-context_switch.dat`.  `analyze` also fixes the second problem by computing
the wait time (time not spent on the process or systems calls relating to the
process).  The formula for wall time (real time) is:
$$\text{wall time} = \frac{\text{user time} + \text{system time} 
                   + \text{wait time}}{\text{processor cores}}$$
which when solved for _wait time_ gives:
$$\text{wait time} = \text{wall time} \cdot \text{processor cores} 
                   - \text{user time} + \text{system time}$$
where _processor cores_ are the number of logical cores. This script gives the
files located in the "Graph Data" section of Appendix A.

The final part of the analysis is generating the plots shown in the next
section.  This is done with the `plot` shell script which makes heavy use of
`gnuplot`.


Testing Setup
-------------

The computer used for benchmarking has the setup shown in Table 1.

Table: Testing computer setup.

| Varaible    | Value                      |
| ----------- | -------------------------- |
| OS          | Xubuntu                    |
| Kernel      | 3.13.0                     |
| Architecture| x86/_64                    |
| CPU         | Intel Core i7-3820         |
| CPU Clock   | 4.00 GHz                   |
| CPU Cores   | 4 (8 Hyper-Threaded)       |
| Memory      | 16 GB                      |
| HDD         | Intel 530 Series SSD       |
| Filesystem  | BTRFS                      |


Results and Analysis
====================

![Wall, user, system, and wait times of the completly fair
sceduler.](plots/cfs-time.pdf)

![Wall, user, system, and wait times of the first come first serve
scheduler.](plots/fcfs-time.pdf)

![Wall, user, system, and wait times of the round robin
scheduler.](plots/rr-time.pdf)


![CPU usage for the completly fair scheduler.](plots/cfs-percent.pdf)

![CPU usage for the first come first serve scheduler.](plots/fcfs-percent.pdf)

![CPU usage for the round robin scheduler.](plots/rr-percent.pdf)


![Context switch count for the completly fair
scheduler.](plots/cfs-context_switch.pdf)

![Context switch count for the first come first serve
scheduler.](plots/fcfs-context_switch.pdf)

![Context switch count for the round robin
scheduler.](plots/rr-context_switch.pdf)

#### Mixed ####
At first glance, all three scheduling policies seemed to behave relatively
 the same. However, the Completely Fair Scheduler (CFS) seemed to be the
fastest. This is not surprising, as most modern Linux 
schedulers use CFS because it works very well. 
What should be of note though are the instances where this is not the case.

In the following cases, overall (wall) CFS times were outperformed:

- RR: CPU bound light-usage
- RR: CPU bound heavy-usage
- RR: IO bound medium-usage
- RR: Mixed light-usage
- FCFS: IO bound light-usage
- FCFS: IO bound medium-usage
- FCFS: Mixed light-usage

These specific cases in which CFS was outperformed should not seem 
unexpected. These instances also seem to stand out as they do not
appear to be very common usage cases. 

Unix (along with most other
OSes) is an interactive operating system. The average user does not
need to know, nor do they likely care, if the system spends less time
on a particular process compared to another scheduling method. The only
tangible result they want is the time from execution to completion of
a particular task. From a pure self-heuristic standpoint I would believe
that most user tasks fall under the *Mixed* category and the scheduling
policy would have been created with that in mind. It does leave questions
remaining as to why CFS was the most inefficient in regards to
mixed processes.

The scheduling policy was most likely developed for the masses, or 
the average user. The average user usually 
performs some interactive task, begins execution, and waits for the results
before performing some other interactive task. 
A common example would be
using a web browser. The user interactively enters the URL of a particular
website they wish to visit and then tells the web browser to execute the 
task of querying the server for whatever page they wish to see. From there,
the web browser waits until its requested data is completely loaded. 
The user then has the web browser perform some other quick task and
once again waits on IO and the cycle repeats. 

Aside from *Mixed light-usage*, the cases when CFS was outran do not 
seem to be very common. It is still very surprising that CFS performed
slower than both RR's and FCFS's cases for *Mixed light-usage*. I would
have expected CFS to be the fastest in that case, as that is what 
seems like would be the most common usage scenario: a user requesting a
small task that may require a small amount of data to be queried from IO.
In actuality, it may be that *Mixed medium-usage* is the most common. 
The common user wants an OS to be snappy. When they click on a button,
they want to see instant gratification of the button appearing pressed. 
If this process does not happen simultaneously, it appears to the user
that there is a problem occurring somewhere. 

The discrepancy stems from CFS's much higher **System Time**. CFS, RR, and
 FCFS have scheduling complexity of O(log n), O(1), and O(1), respectively. 
With this in mind it makes sense that CFS is slow for small tasks when
the scheduling rivals the time of the task itself. CFS scales
nicely though, and its efficiency only increases as
a larger load is applied. 


#### I/O Bound ####
In terms of overall time, First Come First Served is the fastest for
IO bound processes. It isn't that much faster than RR scheduling though
and its CPU efficiency is is approximately the same as RR. The
similar efficiency makes sense, as neither policy have much scheduling to do
aside from starting on the next task in the run queue. With a large number
of similar processes, First Come First Served approximates a Round Robin
policy. This explains why the differences between the two scheduling policies
seem to diminish as more processes are added to the system. 

In networking devices such as routers and switches it would make sense for 
First Come First Served to be used. Most of the packets require very little
computation compared to network latency and are therefore IO bound. 

In CPU intensive applications, FCFS would not fair as well though. 
Throughput can be low, since long processes can hold the CPU. For the 
same reason, turnaround time, waiting time, and response time can also
be high. Long processes can also cause starvation while using FCFS. 

#### CPU Bound ####
In terms of overall time, Round Robbin is the fastest for CPU bound
applications. Its time is on par with CFS for light and medium-usage, but
it outpaces CFS for heavy-usage. What is slightly surprising is that RR has
a lower CPU utilization than CFS. Even more surprising is the fact that CPU
utilization declines as the load is increased for RR CPU bound tasks. In 
all other benchmarks, CPU utilization only increased as a larger load
was applied. 

It is not exactly clear where RR would best be utilized though. If CPU
efficiency was not a concern RR would excel in heavy CPU intensive
applications. In reality, this is usually not the case though.
Round Robbin is however used frequently in "best-effort packet switching"
and other statistical multiplexing. It can also be useful in critical
embedded systems due to the fact that Round Robin is starvation free. 


Conclusion
==========






References
==========


McCormick, Anthony. 2014. “Pandoc Include Files Filter in Haskell.” _Stack Over-
flow_.
[http://stackoverflow.com/questions/21584396/pandoc-include-files-filter-in-haskell](http://stackoverflow.com/questions/21584396/pandoc-include-files-filter-in-haskell).


Richard Han, Rahat Rafiq, Mohammad Hashemi. 2015. "Programing Assign- ment 3:
Investigating the Linux Scheduler." _CSCI 3753 - Operating Systems._ University
of Colorado at Boulder.





Appendices
==========


Appendix A
----------

This appendix contains the raw and partially processed data that was only
displayed in the main report in graph form.

### Raw Data

This is the raw data from each benchmark.  The format is:
```
<wall time> <user time> <system time> <% CPU usage> <involuntary cs> <voluntary cs>
```
where all times are in decimal seconds and `cs` stands for _context switches_.
Each line is a separate iteration of the benchmarks.


##### CFS CPU bound light load (`results/raw/cfs-cpu-light.dat`)

``` {include="./results/raw/cfs-cpu-light.dat"}
```

##### CFS CPU bound medium load (`results/raw/cfs-cpu-medium.dat`)

``` {include="./results/raw/cfs-cpu-medium.dat"}
```

##### CFS CPU bound heavy load (`results/raw/cfs-cpu-heavy.dat`)

``` {include="./results/raw/cfs-cpu-heavy.dat"}
```

##### CFS IO bound light load (`results/raw/cfs-io-light.dat`)

``` {include="./results/raw/cfs-io-light.dat"}
```

##### CFS IO bound medium load (`results/raw/cfs-io-medium.dat`)

``` {include="./results/raw/cfs-io-medium.dat"}
```

##### CFS IO bound heavy load (`results/raw/cfs-io-heavy.dat`)

``` {include="./results/raw/cfs-io-heavy.dat"}
```

##### CFS mixed light load (`results/raw/cfs-mixed-light.dat`)

``` {include="./results/raw/cfs-mixed-light.dat"}
```

##### CFS mixed medium load (`results/raw/cfs-mixed-medium.dat`)

``` {include="./results/raw/cfs-mixed-medium.dat"}
```

##### CFS mixed heavy load (`results/raw/cfs-mixed-heavy.dat`)

``` {include="./results/raw/cfs-mixed-heavy.dat"}
```

##### FCFS CPU bound light load (`results/raw/fcfs-cpu-light.dat`)

``` {include="./results/raw/fcfs-cpu-light.dat"}
```

##### FCFS CPU bound medium load (`results/raw/fcfs-cpu-medium.dat`)

``` {include="./results/raw/fcfs-cpu-medium.dat"}
```

##### FCFS CPU bound heavy load (`results/raw/fcfs-cpu-heavy.dat`)

``` {include="./results/raw/fcfs-cpu-heavy.dat"}
```

##### FCFS IO bound light load (`results/raw/fcfs-io-light.dat`)

``` {include="./results/raw/fcfs-io-light.dat"}
```

##### FCFS IO bound medium load (`results/raw/fcfs-io-medium.dat`)

``` {include="./results/raw/fcfs-io-medium.dat"}
```

##### FCFS IO bound heavy load (`results/raw/fcfs-io-heavy.dat`)

``` {include="./results/raw/fcfs-io-heavy.dat"}
```

##### FCFS mixed light load (`results/raw/fcfs-mixed-light.dat`)

``` {include="./results/raw/fcfs-mixed-light.dat"}
```

##### FCFS mixed medium load (`results/raw/fcfs-mixed-medium.dat`)

``` {include="./results/raw/fcfs-mixed-medium.dat"}
```

##### FCFS mixed heavy load (`results/raw/fcfs-mixed-heavy.dat`)

``` {include="./results/raw/fcfs-mixed-heavy.dat"}
```

##### RR CPU bound light load (`results/raw/rr-cpu-light.dat`)

``` {include="./results/raw/rr-cpu-light.dat"}
```

##### RR CPU bound medium load (`results/raw/rr-cpu-medium.dat`)

``` {include="./results/raw/rr-cpu-medium.dat"}
```

##### RR CPU bound heavy load (`results/raw/rr-cpu-heavy.dat`)

``` {include="./results/raw/rr-cpu-heavy.dat"}
```

##### RR IO bound light load (`results/raw/rr-io-light.dat`)

``` {include="./results/raw/rr-io-light.dat"}
```

##### RR IO bound medium load (`results/raw/rr-io-medium.dat`)

``` {include="./results/raw/rr-io-medium.dat"}
```

##### RR IO bound heavy load (`results/raw/rr-io-heavy.dat`)

``` {include="./results/raw/rr-io-heavy.dat"}
```

##### RR mixed light load (`results/raw/rr-mixed-light.dat`)

``` {include="./results/raw/rr-mixed-light.dat"}
```

##### RR mixed medium load (`results/raw/rr-mixed-medium.dat`)

``` {include="./results/raw/rr-mixed-medium.dat"}
```

##### RR mixed heavy load (`results/raw/rr-mixed-heavy.dat`)

``` {include="./results/raw/rr-mixed-heavy.dat"}
```




### Graph Data

This is the data that is actually plotted in the report above.


#### Time Data

This is the time data plotted above which are averages and/or calculations of
the raw data.  The format is :
```
<wall time> <user time> <system time> <wait time>
```
where times are in seconds and the wait time is computed as shown in the report
above.  This time each line gives a different benchmark result.

##### CFS time data (`results/cfs-time.dat`)

``` {include="./results/cfs-time.dat"}
```

##### FCFS time data (`results/fcfs-time.dat`)

``` {include="./results/fcfs-time.dat"}
```

##### RR time data (`results/rr-time.dat`)

``` {include="./results/rr-time.dat"}
```


#### % CPU Usage Data

This is the CPU usage data plotted above which are averages of the raw data. The
format is:
```
<% cpu usage>
```
This time each line gives a different benchmark result.

##### CFS CPU usage data (`results/cfs-percent.dat`)

``` {include="./results/cfs-percent.dat"}
```

##### FCFS CPU usage data (`results/fcfs-percent.dat`)

``` {include="./results/fcfs-percent.dat"}
```

##### RR CPU usage data (`results/rr-percent.dat`)

``` {include="./results/rr-percent.dat"}
```


#### % Context Switch Data

This is the context switch data plotted above which are averages of the raw
data.  The format is:
```
<involuntary context switches> <voluntary context switches>
```
This time each line gives a different benchmark result.

##### CFS context switch data (`results/cfs-context_switch.dat`)

``` {include="./results/cfs-context_switch.dat"}
```

##### FCFS context switch data (`results/fcfs-context_switch.dat`)

``` {include="./results/fcfs-context_switch.dat"}
```

##### RR context switch data (`results/rr-context_switch.dat`)

``` {include="./results/rr-context_switch.dat"}
```




Appendix B
----------

### Analyzer (`analyze`)

This script converts the raw data to the plotted data in Appendix A.

``` {.bash include="./analyze"}
```

### Benchmark script (`benchmark`)

This script runs the benchmarks that generate the raw data in Appendix A.

``` {.bash include="./benchmark"}
```

### Pandoc include filter (`pandoc-include`)

This is a pandoc filter found at Stack Overflow [@pandoc-include]. This script
runs the benchmarks that generate the raw data in Appendix A.

``` {.haskell include="./pandoc-include"}
```

### Plotting script (`plot`)

The plotting script that generates the PDF files in `plots` by using `gnuplot`.

``` {.bash include="./plot"}
```

### Prime finding program (`prime.c`)

This program generates the first `n` primes.

``` {.c include="./prime.c"}
```

### Prime finding library (`primes.h` and `primes.c`)

This library generates primes.

#### `primes.h`

``` {.c include="./primes.h"}
```

#### `primes.c`

``` {.c include="./primes.c"}
```

### IO bound file copy program (`rw.c`)

This is the `rw` program that came with the assignment handout [@handout].

``` {.c include="./rw.c"}
```

### Scheduling and running program (`schedule.c`)

This program forks of a given number of other processes and runs them with the
given scheduler.

``` {.c include="./schedule.c"}
```


