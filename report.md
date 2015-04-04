% An Analysis of Linux Schedulers
% Michael R. Shannon
% Saturday, April 04, 2015





_Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vel accumsan ipsum, id facilisis dolor. Vivamus a sagittis diam. Nam congue lectus vel lorem tempor egestas. Aliquam consectetur aliquet felis, et lobortis magna malesuada vitae. Donec vitae nibh enim. Pellentesque finibus ex elit, non facilisis sapien ornare id. Phasellus lacus ante, fringilla mollis arcu a, mattis iaculis magna._




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
IO bound code from the lab handout (INSERT REFERENCE).




Methods
=======









Results and Analysis
====================

Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vel accumsan ipsum, id facilisis dolor. Vivamus a sagittis diam. Nam congue lectus vel lorem tempor egestas. Aliquam consectetur aliquet felis, et lobortis magna malesuada vitae. Donec vitae nibh enim. Pellentesque finibus ex elit, non facilisis sapien ornare id. Phasellus lacus ante, fringilla mollis arcu a, mattis iaculis magna.

Nunc volutpat mollis quam, a eleifend eros. Maecenas non nulla lectus. Quisque dapibus metus est, et faucibus enim semper vitae. Nunc lacus ipsum, imperdiet et mattis ac, sodales sed massa. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut sed malesuada tellus. Aliquam sem mauris, euismod id auctor in, sollicitudin in ipsum. Vivamus odio nibh, pulvinar a nunc a, accumsan mattis sapien. Vestibulum pellentesque nulla sit amet eros condimentum molestie. Donec vitae felis eros.

Fusce id urna vitae purus porttitor euismod. Donec elementum lacinia eros quis imperdiet. Mauris non aliquam ligula, congue venenatis nulla. Nulla orci ex, sodales non magna a, tristique sollicitudin tellus. Ut orci arcu, viverra vitae lorem ac, mattis fermentum dui. Nullam risus nisl, aliquam et lectus sit amet, pretium fermentum justo. Aenean elementum, enim non sollicitudin tincidunt, justo lacus volutpat diam, nec fermentum urna nibh vulputate ipsum. Cras in metus rhoncus, tempus velit sed, ultricies massa. Aenean vel diam vel odio blandit egestas eget eget ex. Duis iaculis sapien ac sem auctor volutpat. Donec eu viverra neque.

Vestibulum ut justo non sapien volutpat ullamcorper. Nulla facilisi. Cras ut eros massa. Donec in fringilla metus. Morbi ultrices, quam vitae sollicitudin fermentum, risus enim auctor purus, eu finibus dui felis id ligula. Phasellus blandit ipsum porttitor ex viverra, in fermentum leo ultrices. Nulla facilisi. Nam pharetra dolor eros, sit amet pretium diam sodales venenatis. Maecenas vel viverra urna, eu volutpat turpis. Donec a ligula pulvinar, vulputate nibh vel, semper dolor. Fusce vehicula interdum placerat. Nam lectus libero, lacinia nec magna a, porttitor accumsan est.

Morbi varius purus vel augue hendrerit efficitur. Sed finibus, risus id varius fermentum, diam felis faucibus nisi, vehicula pellentesque ligula enim et mi. Suspendisse tristique et diam sed ornare. Aliquam lectus risus, consectetur sit amet maximus et, malesuada at eros. Fusce fringilla auctor fringilla. Integer diam tellus, fringilla laoreet mi a, pretium cursus ante. Proin eleifend elit a felis feugiat suscipit. Phasellus a dolor sed massa gravida efficitur at in nisi. Aliquam erat volutpat.




Conclusion
==========

Fusce id urna vitae purus porttitor euismod. Donec elementum lacinia eros quis imperdiet. Mauris non aliquam ligula, congue venenatis nulla. Nulla orci ex, sodales non magna a, tristique sollicitudin tellus. Ut orci arcu, viverra vitae lorem ac, mattis fermentum dui. Nullam risus nisl, aliquam et lectus sit amet, pretium fermentum justo. Aenean elementum, enim non sollicitudin tincidunt, justo lacus volutpat diam, nec fermentum urna nibh vulputate ipsum. Cras in metus rhoncus, tempus velit sed, ultricies massa. Aenean vel diam vel odio blandit egestas eget eget ex. Duis iaculis sapien ac sem auctor volutpat. Donec eu viverra neque.

Vestibulum ut justo non sapien volutpat ullamcorper. Nulla facilisi. Cras ut eros massa. Donec in fringilla metus. Morbi ultrices, quam vitae sollicitudin fermentum, risus enim auctor purus, eu finibus dui felis id ligula. Phasellus blandit ipsum porttitor ex viverra, in fermentum leo ultrices. Nulla facilisi. Nam pharetra dolor eros, sit amet pretium diam sodales venenatis. Maecenas vel viverra urna, eu volutpat turpis. Donec a ligula pulvinar, vulputate nibh vel, semper dolor. Fusce vehicula interdum placerat. Nam lectus libero, lacinia nec magna a, porttitor accumsan est.




References
==========

* Reference A
* Reference B



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

This is a pandoc filter found at [StackOverflow: pandoc include files filter in
haskell](http://stackoverflow.com/questions/21584396/pandoc-include-files-filter-in-haskell)
This script runs the benchmarks that generate the raw data in Appendix A.

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

This is the `rw` program that came with the assignment handout (INSERT REFERENCE).

``` {.c include="./rw.c"}
```

### Scheduling and running program (`schedule.c`)

This program forks of a given number of other processes and runs them with the
given scheduler.

``` {.c include="./schedule.c"}
```


