#VampirTrace/Score-P perf Plugin Counter

##Compilation and Installation

###Prerequisites

To compile this plugin, you need:

* C compiler

* CMake

* VampirTrace or Score-P

* A recent Linux kernel (`2.6.31+`) with activated tracing and the kernel headers

###Building

1. Create a build directory

        mkdir build
        cd build

2. Invoke CMake

    Specify the VampirTrace and/or Score-P directory if it is not in the default path with
    `-DVT_INC=<PATH>` respectivly `-DSCOREP_DIR=<PATH>`. The plugin will use alternatively the
    environment variables `VT_DIR` and `SCOREP_DIR`, e.g.

        cmake .. -DVT_DIR=/opt/vampirtrace -DSCOREP_DIR=/opt/scorep

3. Invoke make

        make

4. Copy it to a location listed in `LD_LIBRARY_PATH` or add current path to `LD_LIBRARY_PATH` with

        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`

##Usage

To add a kernel event counter to your trace, you have to specify the environment variable
`VT_PLUGIN_CNTR_METRIC` respectivly `SCOREP_METRIC_PLUGINS`.

`VT_PLUGIN_CNTR_METRIC`/`SCOREP_METRIC_PLUGINS` specifies the software events that shall be recorded
when tracing an application. You can add the following metrics:

| Name                                               | Type                          |
| -------------------------------------------------- | ----------------------------- |
| cpu-cycles *or* cycles                             | Hardware event                |
| stalled-cycles-frontend *or* idle-cycles-frontend  | Hardware event                |
| stalled-cycles-backend *or* idle-cycles-backend    | Hardware event                |
| instructions                                       | Hardware event                |
| cache-references                                   | Hardware event                |
| cache-misses                                       | Hardware event                |
| branch-instructions *or* branches                  | Hardware event                |
| branch-misses                                      | Hardware event                |
| bus-cycles                                         | Hardware event                |
| cpu-clock                                          | Software event                |
| task-clock                                         | Software event                |
| page-faults *or* faults                            | Software event                |
| minor-faults                                       | Software event                |
| major-faults                                       | Software event                |
| context-switches *or* cs                           | Software event                |
| cpu-migrations *or* migrations                     | Software event                |
| alignment-faults                                   | Software event                |
| emulation-faults                                   | Software event                |
| L1-dcache-loads                                    | Hardware cache event          |
| L1-dcache-load-misses                              | Hardware cache event          |
| L1-dcache-stores                                   | Hardware cache event          |
| L1-dcache-store-misses                             | Hardware cache event          |
| L1-dcache-prefetches                               | Hardware cache event          |
| L1-dcache-prefetch-misses                          | Hardware cache event          |
| L1-icache-loads                                    | Hardware cache event          |
| L1-icache-load-misses                              | Hardware cache event          |
| L1-icache-prefetches                               | Hardware cache event          |
| L1-icache-prefetch-misses                          | Hardware cache event          |
| LLC-loads                                          | Hardware cache event          |
| LLC-load-misses                                    | Hardware cache event          |
| LLC-stores                                         | Hardware cache event          |
| LLC-store-misses                                   | Hardware cache event          |
| LLC-prefetches                                     | Hardware cache event          |
| LLC-prefetch-misses                                | Hardware cache event          |
| dTLB-loads                                         | Hardware cache event          |
| dTLB-load-misses                                   | Hardware cache event          |
| dTLB-stores                                        | Hardware cache event          |
| dTLB-store-misses                                  | Hardware cache event          |
| dTLB-prefetches                                    | Hardware cache event          |
| dTLB-prefetch-misses                               | Hardware cache event          |
| iTLB-loads                                         | Hardware cache event          |
| iTLB-load-misses                                   | Hardware cache event          |
| branch-loads                                       | Hardware cache event          |
| branch-load-misses                                 | Hardware cache event          |
| node-loads                                         | Hardware cache event          |
| node-load-misses                                   | Hardware cache event          |
| node-stores                                        | Hardware cache event          |
| node-store-misses                                  | Hardware cache event          |
| node-prefetches                                    | Hardware cache event          |
| node-prefetch-misses                               | Hardware cache event          |
| rNNN (see `perf list --help` on how to encode it)  | Raw hardware event descriptor |


E.g. VampirTrace plugin:

    export VT_PLUGIN_CNTR_METRICS="PerfVT_instructions:PerfVT_L1-dcache-loads"

E.g. Score-P plugin:

    export SCOREP_METRIC_PLUGINS="PerfScore-P_instructions:PerfScore-P_L1-dcache-loads"

###If anything fails

1. Check whether the plugin library can be loaded from the `LD_LIBRARY_PATH`.

2. It may be, that your kernel headers do not provide the file `include/linux/perf_events.h`.
    If thats the case for your system, you should check whether there is the file
    `include/linux/perf_counter.h` and include this file instead. You should also rename the type of
    the variable `attr` in the function `add_event()` to `perf_counter_attr`.

3. Write a mail to the author.

##Authors

* Robert Schoene (robert.schoene at tu-dresden dot de)
