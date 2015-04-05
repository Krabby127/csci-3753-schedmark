% An Analysis of Linux Schedulers
% Michael R. Shannon, Michael N. Eller
% Saturday, April 04, 2015





_Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vel accumsan ipsum,
id facilisis dolor. Vivamus a sagittis diam. Nam congue lectus vel lorem tempor
egestas. Aliquam consectetur aliquet felis, et lobortis magna malesuada vitae.
Donec vitae nibh enim. Pellentesque finibus ex elit, non facilisis sapien ornare
id. Phasellus lacus ante, fringilla mollis arcu a, mattis iaculis magna._




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
of data transfered is 100 blocks by default.

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
soft realtime RR scheduler, or the soft realtime FCFS scheduler (known internaly
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
This script gives the files located in the "Graph Data" section of Appendix A.

The final part of the analysis is generating the plots shown in the next
section.  This is done with the `plot` shell script which makes heavy use of
`gnuplot`.


Testing Setup
-------------

The computer used for benchmarking has the setup shown in Table 1.

Table: Testing computer setup.

| Varaible    | Value                      |
| ----------- | -------------------------- |
| OS          | Arch Linux                 |
| Kernel      | 3.19.2                     |
| Archtecture | i686                       |
| CPU         | Intel Core 2 T7400         |
| CPU Clock   | 2.16 GHz                   |
| CPU Cores   | 2 (no Hyper-Threading)     |
| Memory      | 2 GB                       |
| HDD         | Seagate Barracuda 7200 RPM |
| Filesystem  | BTRFS                      |


The reasoning behind using this old machine was that bare metal could be used
without worrying about a mistake in the code that could destroy data or the OS
itself due to running as root, as this machines installation and data were
disposable.

In addition, to isolate the testing from outside interference no GUI was used.




Results and Analysis
====================

Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vel accumsan ipsum,
id facilisis dolor. Vivamus a sagittis diam. Nam congue lectus vel lorem tempor
egestas. Aliquam consectetur aliquet felis, et lobortis magna malesuada vitae.
Donec vitae nibh enim. Pellentesque finibus ex elit, non facilisis sapien ornare
id. Phasellus lacus ante, fringilla mollis arcu a, mattis iaculis magna.

Nunc volutpat mollis quam, a eleifend eros. Maecenas non nulla lectus. Quisque
dapibus metus est, et faucibus enim semper vitae. Nunc lacus ipsum, imperdiet et
mattis ac, sodales sed massa. Lorem ipsum dolor sit amet, consectetur adipiscing
elit. Ut sed malesuada tellus. Aliquam sem mauris, euismod id auctor in,
sollicitudin in ipsum. Vivamus odio nibh, pulvinar a nunc a, accumsan mattis
sapien. Vestibulum pellentesque nulla sit amet eros condimentum molestie. Donec
vitae felis eros.

Fusce id urna vitae purus porttitor euismod. Donec elementum lacinia eros quis
imperdiet. Mauris non aliquam ligula, congue venenatis nulla. Nulla orci ex,
sodales non magna a, tristique sollicitudin tellus. Ut orci arcu, viverra vitae
lorem ac, mattis fermentum dui. Nullam risus nisl, aliquam et lectus sit amet,
pretium fermentum justo. Aenean elementum, enim non sollicitudin tincidunt,
justo lacus volutpat diam, nec fermentum urna nibh vulputate ipsum. Cras in
metus rhoncus, tempus velit sed, ultricies massa. Aenean vel diam vel odio
blandit egestas eget eget ex. Duis iaculis sapien ac sem auctor volutpat. Donec
eu viverra neque.

Vestibulum ut justo non sapien volutpat ullamcorper. Nulla facilisi. Cras ut
eros massa. Donec in fringilla metus. Morbi ultrices, quam vitae sollicitudin
fermentum, risus enim auctor purus, eu finibus dui felis id ligula. Phasellus
blandit ipsum porttitor ex viverra, in fermentum leo ultrices. Nulla facilisi.
Nam pharetra dolor eros, sit amet pretium diam sodales venenatis. Maecenas vel
viverra urna, eu volutpat turpis. Donec a ligula pulvinar, vulputate nibh vel,
semper dolor. Fusce vehicula interdum placerat. Nam lectus libero, lacinia nec
magna a, porttitor accumsan est.

Morbi varius purus vel augue hendrerit efficitur. Sed finibus, risus id varius
fermentum, diam felis faucibus nisi, vehicula pellentesque ligula enim et mi.
Suspendisse tristique et diam sed ornare. Aliquam lectus risus, consectetur sit
amet maximus et, malesuada at eros. Fusce fringilla auctor fringilla. Integer
diam tellus, fringilla laoreet mi a, pretium cursus ante. Proin eleifend elit a
felis feugiat suscipit. Phasellus a dolor sed massa gravida efficitur at in
nisi. Aliquam erat volutpat.




Conclusion
==========

Fusce id urna vitae purus porttitor euismod. Donec elementum lacinia eros quis
imperdiet. Mauris non aliquam ligula, congue venenatis nulla. Nulla orci ex,
sodales non magna a, tristique sollicitudin tellus. Ut orci arcu, viverra vitae
lorem ac, mattis fermentum dui. Nullam risus nisl, aliquam et lectus sit amet,
pretium fermentum justo. Aenean elementum, enim non sollicitudin tincidunt,
justo lacus volutpat diam, nec fermentum urna nibh vulputate ipsum. Cras in
metus rhoncus, tempus velit sed, ultricies massa. Aenean vel diam vel odio
blandit egestas eget eget ex. Duis iaculis sapien ac sem auctor volutpat. Donec
eu viverra neque.

Vestibulum ut justo non sapien volutpat ullamcorper. Nulla facilisi. Cras ut
eros massa. Donec in fringilla metus. Morbi ultrices, quam vitae sollicitudin
fermentum, risus enim auctor purus, eu finibus dui felis id ligula. Phasellus
blandit ipsum porttitor ex viverra, in fermentum leo ultrices. Nulla facilisi.
Nam pharetra dolor eros, sit amet pretium diam sodales venenatis. Maecenas vel
viverra urna, eu volutpat turpis. Donec a ligula pulvinar, vulputate nibh vel,
semper dolor. Fusce vehicula interdum placerat. Nam lectus libero, lacinia nec
magna a, porttitor accumsan est.




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


