cmake_minimum_required(VERSION 2.8)
project(PerfVT)

#set(CMAKE_C_COMPILER clang)
#plugin source files
set(PLUGIN_SOURCE perf.c)

#additional c flags
set(CMAKE_C_FLAGS "-D_GNU_SOURCE -std=c11")

#debugging c flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHAVE_DEBUG -O0 -Wstrict-prototypes -Wall -Wundef -Wno-long-long -Wsign-compare -Wcomment -pedantic -finline-functions -fno-strict-aliasing")
set(CMAKE_C_FLAGS_RELEASE "-Os")

set(FEATURES 
    PERF_COUNT_HW_STALLED_CYCLES_FRONTEND
    PERF_COUNT_HW_STALLED_CYCLES_BACKEND
    PERF_COUNT_SW_ALIGNMENT_FAULTS
    PERF_COUNT_SW_EMULATION_FAULTS
)

foreach(FEAUTRE ${FEATURES})
    execute_process(COMMAND grep ${FEAUTRE} /usr/include/linux/perf_event.h RESULT_VARIABLE rv OUTPUT_QUIET)
    if(NOT rv)
        add_definitions("-DHAVE_DECL_${FEAUTRE}")
    endif()
endforeach()

include(common/FindVampirTrace.cmake)
if(VT_FOUND)
    add_library(PerfVT SHARED ${PLUGIN_SOURCE})
    set_target_properties(PerfVT PROPERTIES COMPILE_DEFINITIONS VT)
    set_target_properties(PerfVT PROPERTIES INCLUDE_DIRECTORIES "${VT_INCLUDE_DIRS}")
    target_link_libraries(PerfVT pthread m)
endif()

include(common/FindScorep.cmake)
if(SCOREP_FOUND)
    add_library(PerfScoreP SHARED ${PLUGIN_SOURCE})
    set_target_properties(PerfScoreP PROPERTIES COMPILE_DEFINITIONS SCOREP)
    set_target_properties(PerfScoreP PROPERTIES INCLUDE_DIRECTORIES "${SCOREP_INCLUDE_DIRS}")
    target_link_libraries(PerfScoreP pthread m)
endif()
    
if(NOT SCOREP_FOUND AND NOT VT_FOUND)
    message(FATAL_ERROR "Found neither ScoreP or VampirTrace backend!")
endif()
