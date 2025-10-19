#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "scheduler.h"

void schedule_fifo(process_t *processes, int n, timeline_event_t **timeline, int *timeline_len);
void schedule_sjf(process_t *processes, int n, timeline_event_t **timeline, int *timeline_len);
void schedule_stcf(process_t *processes, int n, timeline_event_t **timeline, int *timeline_len);
void schedule_rr(process_t *processes, int n, int quantum, timeline_event_t **timeline, int *timeline_len);

typedef struct {
    int num_queues;
    int *quantums;
    int boost_interval;
} mlfq_config_t;

void schedule_mlfq(process_t *processes, int n, mlfq_config_t *config, timeline_event_t **timeline, int *timeline_len);

#endif // ALGORITHMS_H
