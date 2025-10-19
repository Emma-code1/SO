#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/algorithms.h"

/* helper to clone processes */
static process_t *clone_procs(process_t *p, int n) {
    process_t *c = malloc(sizeof(process_t)*n);
    for(int i=0;i<n;i++) c[i]=p[i];
    return c;
}

/* push event to dynamic timeline */
static void push_event(timeline_event_t **t, int *len, int time, int pid, int duration){
    int newlen = *len + 1;
    *t = realloc(*t, sizeof(timeline_event_t)*newlen);
    (*t)[*len].time = time;
    (*t)[*len].pid = pid;
    (*t)[*len].duration = duration;
    *len = newlen;
}

/* FIFO - non-preemptive */
void schedule_fifo(process_t *processes, int n, timeline_event_t **timeline, int *timeline_len){
    process_t *procs = clone_procs(processes,n);
    for(int i=0;i<n;i++){
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].start_time = -1;
        procs[i].completion_time = -1;
    }
    *timeline = NULL; *timeline_len = 0;
    // sort indices by arrival then pid
    int t = 0;
    int finished = 0;
    while(finished < n){
        int idx = -1;
        for(int i=0;i<n;i++){
            if(procs[i].completion_time != -1) continue;
            if(procs[i].arrival_time <= t){
                if(idx==-1 || procs[i].arrival_time < procs[idx].arrival_time || (procs[i].arrival_time==procs[idx].arrival_time && procs[i].pid < procs[idx].pid)) idx=i;
            }
        }
        if(idx==-1){
            int next = 1e9;
            for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time < next) next = procs[i].arrival_time;
            t = next; continue;
        }
        if(procs[idx].start_time == -1) procs[idx].start_time = t;
        push_event(timeline, timeline_len, t, procs[idx].pid, procs[idx].burst_time);
        t += procs[idx].burst_time;
        procs[idx].remaining_time = 0;
        procs[idx].completion_time = t;
        finished++;
    }
    // copy back start/completion to original array
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) if(processes[i].pid == procs[j].pid){
            processes[i].start_time = procs[j].start_time;
            processes[i].completion_time = procs[j].completion_time;
            processes[i].remaining_time = procs[j].remaining_time;
        }
    }
    free(procs);
}

/* SJF - non preemptive */
void schedule_sjf(process_t *processes, int n, timeline_event_t **timeline, int *timeline_len){
    process_t *procs = clone_procs(processes,n);
    for(int i=0;i<n;i++){
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].start_time = -1;
        procs[i].completion_time = -1;
    }
    *timeline = NULL; *timeline_len = 0;
    int finished = 0, t = 0;
    while(finished < n){
        int idx = -1;
        for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time <= t){
            if(idx==-1 || procs[i].burst_time < procs[idx].burst_time || (procs[i].burst_time==procs[idx].burst_time && procs[i].arrival_time < procs[idx].arrival_time)) idx=i;
        }
        if(idx==-1){
            int next=1e9;
            for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time < next) next = procs[i].arrival_time;
            t = next; continue;
        }
        if(procs[idx].start_time==-1) procs[idx].start_time = t;
        push_event(timeline, timeline_len, t, procs[idx].pid, procs[idx].burst_time);
        t += procs[idx].burst_time;
        procs[idx].remaining_time = 0;
        procs[idx].completion_time = t;
        finished++;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) if(processes[i].pid==procs[j].pid){
            processes[i].start_time = procs[j].start_time;
            processes[i].completion_time = procs[j].completion_time;
            processes[i].remaining_time = procs[j].remaining_time;
        }
    }
    free(procs);
}

/* STCF - preemptive shortest remaining time first */
void schedule_stcf(process_t *processes, int n, timeline_event_t **timeline, int *timeline_len){
    process_t *procs = clone_procs(processes,n);
    for(int i=0;i<n;i++){ procs[i].remaining_time = procs[i].burst_time; procs[i].start_time=-1; procs[i].completion_time=-1; }
    *timeline = NULL; *timeline_len = 0;
    int t=0, finished=0;
    int current_idx=-1, slice_start=0;
    while(finished<n){
        int idx=-1;
        for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time<=t){
            if(idx==-1 || procs[i].remaining_time < procs[idx].remaining_time) idx=i;
        }
        if(idx==-1){
            int next=1e9;
            for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time < next) next = procs[i].arrival_time;
            t = next; continue;
        }
        if(current_idx != idx){
            if(current_idx!=-1){
                push_event(timeline, timeline_len, slice_start, procs[current_idx].pid, t - slice_start);
            }
            current_idx = idx;
            slice_start = t;
            if(procs[idx].start_time==-1) procs[idx].start_time = t;
        }
        procs[idx].remaining_time--;
        t++;
        if(procs[idx].remaining_time==0){
            procs[idx].completion_time = t;
            push_event(timeline, timeline_len, slice_start, procs[idx].pid, t - slice_start);
            current_idx = -1;
            finished++;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) if(processes[i].pid==procs[j].pid){
            processes[i].start_time = procs[j].start_time;
            processes[i].completion_time = procs[j].completion_time;
            processes[i].remaining_time = procs[j].remaining_time;
        }
    }
    free(procs);
}

/* Round Robin */
void schedule_rr(process_t *processes, int n, int quantum, timeline_event_t **timeline, int *timeline_len){
    process_t *procs = clone_procs(processes,n);
    for(int i=0;i<n;i++){ procs[i].remaining_time = procs[i].burst_time; procs[i].start_time=-1; procs[i].completion_time=-1; }
    *timeline = NULL; *timeline_len = 0;
    int t=0, finished=0;
    int queue_cap = n*4;
    int *q = malloc(sizeof(int)*queue_cap);
    int qh=0, qt=0;
    // helper to enqueue unique indices
    auto enqueue_if_new = [&](int idx){
        for(int j=qh;j<qt;j++) if(q[j%queue_cap]==idx) return;
        q[qt++ % queue_cap] = idx;
    };
    while(finished < n){
        for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time<=t) enqueue_if_new(i);
        if(qh==qt){
            int next=1e9;
            for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time < next) next=procs[i].arrival_time;
            t = next; continue;
        }
        int idx = q[qh++ % queue_cap];
        if(procs[idx].start_time==-1) procs[idx].start_time = t;
        int run = procs[idx].remaining_time < quantum ? procs[idx].remaining_time : quantum;
        push_event(timeline, timeline_len, t, procs[idx].pid, run);
        procs[idx].remaining_time -= run;
        t += run;
        for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time>=(t-run) && procs[i].arrival_time<=t) enqueue_if_new(i);
        if(procs[idx].remaining_time>0) q[qt++ % queue_cap] = idx;
        else { procs[idx].completion_time = t; finished++; }
    }
    free(q);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) if(processes[i].pid==procs[j].pid){
            processes[i].start_time = procs[j].start_time;
            processes[i].completion_time = procs[j].completion_time;
            processes[i].remaining_time = procs[j].remaining_time;
        }
    }
    free(procs);
}

/* MLFQ - simple */
void schedule_mlfq(process_t *processes, int n, mlfq_config_t *config, timeline_event_t **timeline, int *timeline_len){
    process_t *procs = clone_procs(processes,n);
    for(int i=0;i<n;i++){ procs[i].remaining_time = procs[i].burst_time; procs[i].start_time=-1; procs[i].completion_time=-1; }
    *timeline = NULL; *timeline_len = 0;
    int t=0, finished=0;
    int Q = config->num_queues;
    int capacity = n*10;
    int *queues = calloc(Q*capacity, sizeof(int));
    int *qh = calloc(Q,sizeof(int));
    int *qt = calloc(Q,sizeof(int));
    auto qenqueue = [&](int level, int idx){ queues[(qt[level]++)%capacity + level*capacity] = idx; };
    auto qdequeue = [&](int level)->int{ return queues[(qh[level]++)%capacity + level*capacity]; };
    while(finished < n){
        for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time<=t){
            // enqueue if not present
            int present=0;
            for(int L=0;L<Q;L++) for(int j=qh[L]; j<qt[L]; j++) if(queues[(j%capacity)+L*capacity]==i) present=1;
            if(!present) qenqueue(0,i);
        }
        int level = -1;
        for(int L=0;L<Q;L++) if(qh[L] < qt[L]) { level=L; break; }
        if(level==-1){
            int next=1e9; for(int i=0;i<n;i++) if(procs[i].completion_time==-1 && procs[i].arrival_time < next) next=procs[i].arrival_time;
            t = next; continue;
        }
        int idx = qdequeue(level);
        if(procs[idx].start_time==-1) procs[idx].start_time = t;
        int quantum = config->quantums[level];
        int run = procs[idx].remaining_time < quantum ? procs[idx].remaining_time : quantum;
        push_event(timeline, timeline_len, t, procs[idx].pid, run);
        procs[idx].remaining_time -= run;
        t += run;
        if(procs[idx].remaining_time>0){
            int next_lvl = (level+1 < Q)? level+1 : level;
            qenqueue(next_lvl, idx);
        } else {
            procs[idx].completion_time = t; finished++;
        }
    }
    free(queues); free(qh); free(qt);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) if(processes[i].pid==procs[j].pid){
            processes[i].start_time = procs[j].start_time;
            processes[i].completion_time = procs[j].completion_time;
            processes[i].remaining_time = procs[j].remaining_time;
        }
    }
    free(procs);
}
