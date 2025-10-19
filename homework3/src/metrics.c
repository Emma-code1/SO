#include <stdlib.h>
#include "../include/metrics.h"
#include <math.h>

void calculate_metrics(process_t *processes, int n, int total_time, metrics_t *metrics){
    double sum_tat=0, sum_wt=0, sum_rt=0;
    double busy=0;
    double xi_sum=0, xi_sq_sum=0;
    int completed = 0;
    for(int i=0;i<n;i++){
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        processes[i].response_time = processes[i].start_time - processes[i].arrival_time;
        sum_tat += processes[i].turnaround_time;
        sum_wt += processes[i].waiting_time;
        sum_rt += processes[i].response_time;
        if(processes[i].completion_time > 0) completed++;
        xi_sum += processes[i].turnaround_time;
        xi_sq_sum += (double)processes[i].turnaround_time * processes[i].turnaround_time;
        busy += processes[i].burst_time;
    }
    metrics->avg_turnaround_time = sum_tat / n;
    metrics->avg_waiting_time = sum_wt / n;
    metrics->avg_response_time = sum_rt / n;
    metrics->cpu_utilization = (busy / (double)total_time) * 100.0;
    metrics->throughput = (double)completed / (double)total_time;
    if(n*xi_sq_sum != 0) metrics->fairness_index = (xi_sum*xi_sum) / (n * xi_sq_sum);
    else metrics->fairness_index = 1.0;
}
