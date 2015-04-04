#!/bin/sh


# Program to plot benchmarks.
#
# Copyright (C) 2015  Michael R. Shannon
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.




plot()
{
    # store the current directory
    CODE_DIR=$PWD;

    # make temporary directory for output files
    TMP_DIR=$PWD/tmp
    if [ ! -d "$TMP_DIR" ]; then
        mkdir "$TMP_DIR"
    fi

    # make plots directory
    PLOTS_DIR=$PWD/plots
    if [ ! -d "$PLOTS_DIR" ]; then
        mkdir "$PLOTS_DIR"
    fi

    # make time plots
    plot_time $TMP_DIR "./results/cfs-time.dat" "$PLOTS_DIR/cfs-time.pdf"
    plot_time $TMP_DIR "./results/fcfs-time.dat" "$PLOTS_DIR/fcfs-time.pdf"
    plot_time $TMP_DIR "./results/rr-time.dat" "$PLOTS_DIR/rr-time.pdf"

    # make percent plots
    plot_percent $TMP_DIR "./results/cfs-percent.dat" "$PLOTS_DIR/cfs-percent.pdf"
    plot_percent $TMP_DIR "./results/fcfs-percent.dat" "$PLOTS_DIR/fcfs-percent.pdf"
    plot_percent $TMP_DIR "./results/rr-percent.dat" "$PLOTS_DIR/rr-percent.pdf"

    # make context switch plot
    plot_context_switch $TMP_DIR "./results/cfs-context_switch.dat" \
                                 "$PLOTS_DIR/cfs-context_switch.pdf"
    plot_context_switch $TMP_DIR "./results/fcfs-context_switch.dat" \
                                 "$PLOTS_DIR/fcfs-context_switch.pdf"
    plot_context_switch $TMP_DIR "./results/rr-context_switch.dat" \
                                 "$PLOTS_DIR/rr-context_switch.pdf"
}




plot_time()
{
    PLOT_FILE="$1/plot.gnuplot"

    # convert to gnuplot friendly data format
    awk '{
        offset+=1;
        printf(\
            " %12.6f %12.6f\n %12.6f %12.6f\n %12.6f %12.6f\n %12.6f %12.6f\n\n\n",
            offset-0.21, $1,
            offset-0.07, $2,
            offset+0.07, $3,
            offset+0.21, $4)
    }' "$2" > "$PLOT_FILE"

    # plot the data
    gnuplot -persist <<EOF

set terminal pdf
set output "$3"

set xtics ("L-CPU" 1, \
           "M-CPU" 2, \
           "H-CPU" 3, \
           "L-IO" 4, \
           "M-IO" 5, \
           "H-IO" 6, \
           "L-Mixed" 7, \
           "M-Mixed" 8, \
           "H-Mixed" 9, \
          )

set ylabel "seconds"
set key left top

set grid
set logscale y
set yrange [0.001:1 < *]

set boxwidth 0.14
set style fill solid

plot "$PLOT_FILE" every 4::0 using 1:2 with boxes ls 1 title "wall time",\
     "$PLOT_FILE" every 4::1 using 1:2 with boxes ls 2 title "user time",\
     "$PLOT_FILE" every 4::2 using 1:2 with boxes ls 3 title "system time",\
     "$PLOT_FILE" every 4::3 using 1:2 with boxes ls 4 title "wait time"
quit
EOF
}




plot_percent()
{
    PLOT_FILE="$1/plot.gnuplot"

    # convert to gnuplot friendly data format
    awk '{
        offset+=1;
        printf(" %12.6f %12.6f\n\n\n", offset, $1)
    }' "$2" > "$PLOT_FILE"

    # plot the data
    gnuplot -persist <<EOF

set terminal pdf
set output "$3"

set xtics ("L-CPU" 1, \
           "M-CPU" 2, \
           "H-CPU" 3, \
           "L-IO" 4, \
           "M-IO" 5, \
           "H-IO" 6, \
           "L-Mixed" 7, \
           "M-Mixed" 8, \
           "H-Mixed" 9, \
          )

set ylabel "% CPU"
set key left top

set grid
set yrange [0:100]

set boxwidth 0.5
set style fill solid

plot "$PLOT_FILE" using 1:2 with boxes ls 1 title ""

quit
EOF
}




plot_context_switch()
{
    PLOT_FILE="$1/plot.gnuplot"

    # convert to gnuplot friendly data format
    awk '{
        offset+=1;
        printf(" %12.6f %12.6f\n %12.6f %12.6f\n\n\n",
               offset-0.15, $1,
               offset+0.15, $2)
    }' "$2" > "$PLOT_FILE"

    # plot the data
    gnuplot -persist <<EOF

set terminal pdf
set output "$3"

set xtics ("L-CPU" 1, \
           "M-CPU" 2, \
           "H-CPU" 3, \
           "L-IO" 4, \
           "M-IO" 5, \
           "H-IO" 6, \
           "L-Mixed" 7, \
           "M-Mixed" 8, \
           "H-Mixed" 9, \
          )

set ylabel "context switches"
set key left top

set grid
set logscale y
set yrange [1:1 < *]

set boxwidth 0.14
set style fill solid

plot "$PLOT_FILE" every 2::0 using 1:2 with boxes ls 1 title "involuntary",\
     "$PLOT_FILE" every 2::1 using 1:2 with boxes ls 2 title "voluntary"
quit
EOF
}




plot $@
