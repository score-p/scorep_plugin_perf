/*
 * Copyright (c) 2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef VT
#include <vampirtrace/vt_plugin_cntr.h>
#else
#ifdef SCOREP
#include <scorep/SCOREP_MetricPlugins.h>
#else
#error "You need Score-P or VampirTrace to compile this plugin"
#endif /* SCOREP*/
#endif /* VT */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/syscall.h>

#include <linux/perf_event.h>


/* init and fini do not do anything */
/* This is intended! */
int32_t init(){
  return 0;
}
void fini(){
  /* we do not close perfs file descriptors */
  /* as we do not store this information */
}

/* This function writes the attr definitions for a given event name
 * If the event has not been found, attr->type is PERF_TYPE_MAX
 * */
void build_perf_attr(struct perf_event_attr * attr, char * name){
  int                i, j, k;
  memset( attr, 0, sizeof( struct perf_event_attr ) );
  attr->config1 = 0;
  attr->config2 = 0;
  attr->type    = PERF_TYPE_MAX;
  /* hardware events */
  if ( ( strstr( name, "cpu-cycles" ) == name ) || ( strstr( name, "cycles" ) == name ) )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_CPU_CYCLES;
  }
#if HAVE_DECL_PERF_COUNT_HW_STALLED_CYCLES_FRONTEND
  else
  if ( ( strstr( name, "stalled-cycles-frontend" ) == name ) || ( strstr( name, "idle-cycles-frontend" ) == name ) )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND;
  }
#endif /* HAVE_DECL_PERF_COUNT_HW_STALLED_CYCLES_FRONTEND */
#if HAVE_DECL_PERF_COUNT_HW_STALLED_CYCLES_BACKEND
  else
  if ( ( strstr( name, "stalled-cycles-backend" ) == name ) || ( strstr( name, "idle-cycles-backend" ) == name ) )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND;
  }
#endif /* HAVE_DECL_PERF_COUNT_HW_STALLED_CYCLES_BACKEND */
  else
  if ( strstr( name, "instructions" ) == name )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_INSTRUCTIONS;
  }
  if ( strstr( name, "cache-references" ) == name )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_CACHE_REFERENCES;
  }
  if ( strstr( name, "cache-misses" ) == name )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_CACHE_MISSES;
  }
  if ( ( strstr( name, "branch-instructions" ) == name ) || ( strstr( name, "branches" ) == name ) )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
  }
  if ( strstr( name, "branch-misses" ) == name )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_BRANCH_MISSES;
  }
  if ( strstr( name, "bus-cycles" ) == name )
  {
    attr->type   = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_BUS_CYCLES;
  }

  /* software events */
  if ( strstr( name, "cpu-clock" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_CPU_CLOCK;
  }
  if ( strstr( name, "task-clock" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_TASK_CLOCK;
  }
  if ( ( strstr( name, "page-faults" ) == name ) || ( strstr( name, "faults" ) == name ) )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_PAGE_FAULTS;
  }
  if ( strstr( name, "minor-faults" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_PAGE_FAULTS_MIN;
  }
  if ( strstr( name, "major-faults" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_PAGE_FAULTS_MAJ;
  }
  if ( strstr( name, "major-faults" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_PAGE_FAULTS_MAJ;
  }
  if ( ( strstr( name, "context-switches" ) == name ) || ( strstr( name, "cs" ) == name ) )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_CONTEXT_SWITCHES;
  }
  if ( ( strstr( name, "cpu-migrations" ) == name ) || ( strstr( name, "migrations" ) == name ) )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_CPU_MIGRATIONS;
  }
#if HAVE_DECL_PERF_COUNT_SW_ALIGNMENT_FAULTS
  if ( strstr( name, "alignment-faults" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_ALIGNMENT_FAULTS;
  }
#endif /* HAVE_DECL_PERF_COUNT_SW_ALIGNMENT_FAULTS */
#if HAVE_DECL_PERF_COUNT_SW_EMULATION_FAULTS
  if ( strstr( name, "emulation-faults" ) == name )
  {
    attr->type   = PERF_TYPE_SOFTWARE;
    attr->config = PERF_COUNT_SW_EMULATION_FAULTS;
  }
#endif /* PERF_COUNT_SW_EMULATION_FAULTS */

  /* cache events */
  /* three levels: component, access type, miss vs hit */
  char  buffer[ 128 ];
  int   num_chars    = 0;
  char* caches[ 7 ]  = { "L1-dcache-", "L1-icache-", "LLC-", "dTLB-", "iTLB-", "branch-", "node-" };
  int   num_caches   = 7;
  char* actions[ 3 ] = { "load", "store", "prefetch" };
  int   num_actions  = 2;
  char* results[ 3 ] = { "s", "-misses", "es" };
  int   num_results  = 3;
  /* Loops that combine the 3 levels */
  for ( i = 0; i < num_caches; i++ )
  {
      if ( strstr( name, caches[ i ] ) == name )
      {
          for ( j = 0; j < num_actions; j++ )
          {
              for ( k = 0; k < num_results; k++ )
              {
                  num_chars = snprintf( buffer, 127, "%s%s%s", caches[ i ], actions[ j ], results[ k ] );
                  /* TODO assert that num_chars is < 127 */
                  if ( strstr( name, buffer ) == name )
                  {
                    attr->type   = PERF_TYPE_HW_CACHE;
                    attr->config = i << 16 | j << 8;
                      if ( k == 1 )
                      {
                        attr->config |= 1;
                      }
                  }
              }
          }
      }
  }
  /* raw events */
  if ( name[ 0 ] == 'r' )
  {
      /* get event */
      attr->type   = PERF_TYPE_RAW;
      attr->config = strtoll( &( name[ 1 ] ), NULL, 16 );
  }
}
/* registers perf event */
int32_t add_counter(char * event_name){
  int fd;
  struct perf_event_attr attr;

  build_perf_attr(&attr, event_name);
  /* wrong metric */
  if (attr.type==PERF_TYPE_MAX){
    fprintf(stderr, "PERF metric not recognized: %s", event_name );
    return -1;
  }
  fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
  if (fd<=0){
    fprintf(stderr, "Unable to open counter \"%s\". Aborting.\n",event_name);
    return -1;
  }
  return fd;
}

/* reads value */
uint64_t get_value(int fd){
  uint64_t count=0;
  size_t res=read(fd, &count, sizeof(uint64_t));
  if (res!=sizeof(uint64_t))
    return !0;
  return count;
}

#ifdef VT

vt_plugin_cntr_metric_info * get_event_info(char * event_name){
    vt_plugin_cntr_metric_info * return_values;

  struct perf_event_attr attr;
  build_perf_attr(&attr, event_name);
  /* wrong metric */
  if (attr.type==PERF_TYPE_MAX){
    fprintf(stderr, "PERF metric not recognized: %s", event_name );
    return NULL;
  }
  return_values=
            malloc(2 * sizeof(vt_plugin_cntr_metric_info) );
  if (return_values==NULL){
            fprintf(stderr, "VampirTrace Perf Plugin: failed to allocate memory for passing information to VT.\n");
            return NULL;
          }
    return_values[0].name=strdup(event_name);
    return_values[0].unit=NULL;
    return_values[0].cntr_property=VT_PLUGIN_CNTR_LAST|VT_PLUGIN_CNTR_START|
     VT_PLUGIN_CNTR_UNSIGNED;
    return_values[1].name=NULL;
    return return_values;
}


/**
 * This function get called to give some informations about the plugin to VT
 */
vt_plugin_cntr_info get_info(){
  vt_plugin_cntr_info info;
  memset(&info,0,sizeof(vt_plugin_cntr_info));
  info.init              = init;
  info.add_counter       = add_counter;
  info.vt_plugin_cntr_version = VT_PLUGIN_CNTR_VERSION;
  info.run_per           = VT_PLUGIN_CNTR_PER_THREAD;
  info.synch             = VT_PLUGIN_CNTR_SYNCH;
  info.get_event_info    = get_event_info;
  info.get_current_value = get_value;
  info.finalize          = fini;
  return info;
}
#elif SCOREP

SCOREP_Metric_Plugin_MetricProperties * get_event_info(char * event_name)
{
  SCOREP_Metric_Plugin_MetricProperties * return_values;

  struct perf_event_attr attr;
  build_perf_attr(&attr, event_name);
  /* wrong metric */
  if (attr.type==PERF_TYPE_MAX){
    fprintf(stderr, "PERF metric not recognized: %s", event_name );
    return NULL;
  }
  return_values=
      malloc(2 * sizeof(SCOREP_Metric_Plugin_MetricProperties) );
  if (return_values==NULL){
        fprintf(stderr, "Score-P Perf Plugin: failed to allocate memory for passing information to Score-P.\n");
        return NULL;
  }
  return_values[0].name        = strdup(event_name);
  return_values[0].unit        = NULL;
  return_values[0].description = NULL;
  return_values[0].mode        = SCOREP_METRIC_MODE_ACCUMULATED_START;
  return_values[0].value_type  = SCOREP_METRIC_VALUE_UINT64;
  return_values[0].base        = SCOREP_METRIC_BASE_DECIMAL;
  return_values[0].exponent    = 0;
  return_values[1].name=NULL;
  return return_values;
}

bool get_optional_value( int32_t   id,
                               uint64_t* value ){
  *value=get_value(id);
  return true;
}

/**
 * This function get called to give some informations about the plugin to scorep
 */
SCOREP_METRIC_PLUGIN_ENTRY( perf_plugin )
{
    /* Initialize info data (with zero) */
    SCOREP_Metric_Plugin_Info info;
    memset( &info, 0, sizeof( SCOREP_Metric_Plugin_Info ) );

    /* Set up the structure */
    info.plugin_version               = SCOREP_METRIC_PLUGIN_VERSION;
    info.run_per                      = SCOREP_METRIC_PER_THREAD;
    info.sync                         = SCOREP_METRIC_SYNC;
    info.initialize                   = init;
    info.finalize                     = fini;
    info.get_event_info               = get_event_info;
    info.add_counter                  = add_counter;
    info.get_current_value            = get_value;
    info.get_optional_value           = get_optional_value;

    return info;
}
#endif /* SCOREP */
