#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/algorithms.h"

/* Read workload file: lines "pid arrival burst priority" */
int read_workload(const char *path, process_t **out){
    FILE *f = fopen(path,"r");
    if(!f) return -1;
    process_t *arr = NULL;
    int len=0;
    while(1){
        int pid,a,b,p;
        int r = fscanf(f,"%d %d %d %d",&pid,&a,&b,&p);
        if(r!=4) break;
        arr = realloc(arr, sizeof(process_t)*(len+1));
        arr[len].pid = pid; arr[len].arrival_time = a; arr[len].burst_time = b; arr[len].priority = p;
        arr[len].remaining_time = b; arr[len].start_time=-1; arr[len].completion_time=-1;
        len++;
    }
    fclose(f);
    *out = arr;
    return len;
}

/* Run algorithm by name; returns timeline via pointers */
int run_algorithm_by_name(const char *name, process_t *processes, int n, int quantum, mlfq_config_t *mlfq_config, timeline_event_t **timeline, int *timeline_len){
    *timeline = NULL; *timeline_len = 0;
    if(strcmp(name,"FIFO")==0){
        schedule_fifo(processes,n,timeline,timeline_len);
    } else if(strcmp(name,"SJF")==0){
        schedule_sjf(processes,n,timeline,timeline_len);
    } else if(strcmp(name,"STCF")==0){
        schedule_stcf(processes,n,timeline,timeline_len);
    } else if(strcmp(name,"RR")==0){
        schedule_rr(processes,n,quantum,timeline,timeline_len);
    } else if(strcmp(name,"MLFQ")==0){
        schedule_mlfq(processes,n,mlfq_config,timeline,timeline_len);
    } else {
        return -1;
    }
    return 0;
}
