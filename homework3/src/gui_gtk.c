#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/algorithms.h"
#include "../include/metrics.h"

/* minimal app widgets struct */
typedef struct { GtkWidget *textview; GtkWidget *combo; GtkWidget *quantum; GtkWidget *mlfq; GtkWidget *drawing; GtkWidget *metrics; } app_widgets;

static process_t *parse_textbuffer_to_procs(GtkTextBuffer *buf, int *out_n){
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buf, &start, &end);
    char *text = gtk_text_buffer_get_text(buf, &start, &end, FALSE);
    process_t *arr = NULL; int len=0;
    char *line = strtok(text,"\n");
    while(line){
        int pid,a,b,p;
        if(sscanf(line,"%d %d %d %d",&pid,&a,&b,&p)==4){
            arr = realloc(arr,sizeof(process_t)*(len+1));
            arr[len].pid=pid; arr[len].arrival_time=a; arr[len].burst_time=b; arr[len].priority=p;
            arr[len].remaining_time=b; arr[len].start_time=-1; arr[len].completion_time=-1;
            len++;
        }
        line = strtok(NULL,"\n");
    }
    g_free(text);
    *out_n = len;
    return arr;
}

struct drawdata { timeline_event_t *events; int len; process_t *procs; int n; };

static void draw_gantt(GtkWidget *widget, cairo_t *cr, struct drawdata *d){
    if(!d || d->len==0) return;
    int total_end=0;
    for(int i=0;i<d->len;i++) total_end = d->events[i].time + d->events[i].duration;
    int W = gtk_widget_get_allocated_width(widget);
    int H = gtk_widget_get_allocated_height(widget);
    double scale = (double)W / (double)(total_end+1);
    cairo_set_source_rgb(cr, 0.98,0.98,0.98);
    cairo_paint(cr);
    for(int i=0;i<d->len;i++){
        timeline_event_t e = d->events[i];
        double x = e.time * scale;
        double w = e.duration * scale;
        unsigned int h = e.pid * 2654435761u;
        double r = ((h>>16)&0xFF)/255.0;
        double g = ((h>>8)&0xFF)/255.0;
        double b = (h&0xFF)/255.0;
        cairo_set_source_rgb(cr, r, g, b);
        cairo_rectangle(cr, x, 10, w, H-30);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0,0,0);
        cairo_rectangle(cr, x, 10, w, H-30);
        cairo_stroke(cr);
        char label[32]; snprintf(label,32,"P%d", e.pid);
        cairo_move_to(cr, x+2, 30);
        cairo_show_text(cr, label);
    }
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    struct drawdata *d = g_object_get_data(G_OBJECT(widget), "drawdata");
    draw_gantt(widget, cr, d);
    return FALSE;
}

static void on_run_clicked(GtkButton *btn, gpointer user_data){
    app_widgets *w = user_data;
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->textview));
    int n; process_t *procs = parse_textbuffer_to_procs(buf, &n);
    if(n==0){
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->metrics)), "No processes parsed.\n", -1);
        return;
    }
    char *alg = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(w->combo));
    int quantum = atoi(gtk_entry_get_text(GTK_ENTRY(w->quantum)));
    char *mlfq_txt = (char*)gtk_entry_get_text(GTK_ENTRY(w->mlfq));
    mlfq_config_t config; config.num_queues=0; config.quantums=NULL; config.boost_interval=0;
    if(strcmp(alg,"MLFQ")==0){
        char *s = strdup(mlfq_txt); char *tok = strtok(s,",");
        while(tok){ config.quantums = realloc(config.quantums, sizeof(int)*(config.num_queues+1));
            config.quantums[config.num_queues++] = atoi(tok); tok = strtok(NULL,","); }
        free(s);
        if(config.num_queues==0){ config.num_queues=3; config.quantums = malloc(sizeof(int)*3); config.quantums[0]=2; config.quantums[1]=4; config.quantums[2]=8; }
    }
    timeline_event_t *timeline = NULL; int timeline_len=0;
    process_t *proc_copy = malloc(sizeof(process_t)*n); for(int i=0;i<n;i++) proc_copy[i]=procs[i];
    run_algorithm_by_name(alg, proc_copy, n, quantum, &config, &timeline, &timeline_len);
    int total_end = 0; for(int i=0;i<timeline_len;i++) total_end = timeline[i].time + timeline[i].duration;
    metrics_t metrics; calculate_metrics(proc_copy, n, total_end>0?total_end:1, &metrics);
    char buf_metrics[512];
    snprintf(buf_metrics,512,"Algorithm: %s\nTotal time: %d\nAvg Turnaround: %.2f\nAvg Waiting: %.2f\nAvg Response: %.2f\nCPU Util: %.2f%%\nThroughput: %.4f\nFairness: %.4f\n",
        alg, total_end, metrics.avg_turnaround_time, metrics.avg_waiting_time, metrics.avg_response_time, metrics.cpu_utilization, metrics.throughput, metrics.fairness_index);
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->metrics)), buf_metrics, -1);
    struct drawdata *d = malloc(sizeof(*d));
    d->events = timeline; d->len = timeline_len; d->procs = proc_copy; d->n = n;
    g_object_set_data_full(G_OBJECT(w->drawing), "drawdata", d, free);
    gtk_widget_queue_draw(w->drawing);
    g_free(alg);
    if(config.quantums) free(config.quantums);
    free(procs);
}

int main(int argc, char **argv){
    gtk_init(&argc, &argv);
    app_widgets *w = g_malloc(sizeof(app_widgets));
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "CPU Scheduler Simulator - GTK");
    gtk_window_set_default_size(GTK_WINDOW(win), 900, 500);
    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_container_add(GTK_CONTAINER(win), hbox);
    GtkWidget *vleft = gtk_box_new(GTK_ORIENTATION_VERTICAL,6);
    gtk_box_pack_start(GTK_BOX(hbox), vleft, TRUE, TRUE, 6);
    GtkWidget *label = gtk_label_new("Processes (pid arrival burst priority)");
    gtk_box_pack_start(GTK_BOX(vleft), label, FALSE, FALSE, 2);
    w->textview = gtk_text_view_new();
    gtk_text_view_set_wrap(GTK_TEXT_VIEW(w->textview), GTK_WRAP_NONE);
    gtk_widget_set_size_request(w->textview, 400, 200);
    gtk_box_pack_start(GTK_BOX(vleft), w->textview, TRUE, TRUE, 2);
    GtkWidget *alg_label = gtk_label_new("Algorithm:");
    gtk_box_pack_start(GTK_BOX(vleft), alg_label, FALSE, FALSE, 2);
    w->combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w->combo), "FIFO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w->combo), "SJF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w->combo), "STCF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w->combo), "RR");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w->combo), "MLFQ");
    gtk_combo_box_set_active(GTK_COMBO_BOX(w->combo), 0);
    gtk_box_pack_start(GTK_BOX(vleft), w->combo, FALSE, FALSE, 2);
    GtkWidget *hparams = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start(GTK_BOX(vleft), hparams, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(hparams), gtk_label_new("Quantum (RR):"), FALSE, FALSE, 2);
    w->quantum = gtk_entry_new(); gtk_entry_set_text(GTK_ENTRY(w->quantum), "3");
    gtk_box_pack_start(GTK_BOX(hparams), w->quantum, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(vleft), gtk_label_new("MLFQ quantums (comma sep. e.g. 2,4,8)"), FALSE, FALSE, 2);
    w->mlfq = gtk_entry_new(); gtk_entry_set_text(GTK_ENTRY(w->mlfq), "2,4,8");
    gtk_box_pack_start(GTK_BOX(vleft), w->mlfq, FALSE, FALSE, 2);
    GtkWidget *run = gtk_button_new_with_label("Run"); gtk_box_pack_start(GTK_BOX(vleft), run, FALSE, FALSE, 2);
    GtkWidget *vright = gtk_box_new(GTK_ORIENTATION_VERTICAL,6);
    gtk_box_pack_start(GTK_BOX(hbox), vright, TRUE, TRUE, 6);
    w->drawing = gtk_drawing_area_new(); gtk_widget_set_size_request(w->drawing, 450, 250); gtk_box_pack_start(GTK_BOX(vright), w->drawing, TRUE, TRUE, 2);
    g_signal_connect(G_OBJECT(w->drawing), "draw", G_CALLBACK(on_draw_event), NULL);
    w->metrics = gtk_text_view_new(); gtk_text_view_set_editable(GTK_TEXT_VIEW(w->metrics), FALSE); gtk_widget_set_size_request(w->metrics, 450, 180);
    gtk_box_pack_start(GTK_BOX(vright), w->metrics, FALSE, FALSE, 2);
    g_signal_connect(run, "clicked", G_CALLBACK(on_run_clicked), w);
    const char *sample = "1 0 5 1\n2 1 3 2\n3 2 8 1\n4 3 6 3\n5 5 2 2\n";
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->textview)), sample, -1);
    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
