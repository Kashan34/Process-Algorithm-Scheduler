#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int priority;
    int completion_time;
    int waiting_time; // Added field for waiting time
} Process;

Process processes[10];
int num_processes = 0;
GtkWidget *burst_entry;
GtkWidget *arrival_entry;
GtkWidget *priority_entry;

void clear_processes() {
    num_processes = 0;
}

void reset_processes() {
    memset(processes, 0, sizeof(processes));
    num_processes = 0;
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Processes have been reset.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void display_message_dialog(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

double calculate_avg_waiting_time() {
    int total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    return (double)total_waiting_time / num_processes;
}

void display_scheduling_result(const char *result) {
    gchar result_message[512];
    g_snprintf(result_message, sizeof(result_message), "%s\n\nScheduling Order:\n", result);

    for (int i = 0; i < num_processes; i++) {
        gchar process_info[128];
        g_snprintf(process_info, sizeof(process_info), "Process ID: %d, Waiting Time: %d\n", processes[i].process_id, processes[i].waiting_time);
        strcat(result_message, process_info);
    }

    double avg_waiting_time = calculate_avg_waiting_time();
    gchar avg_waiting_time_str[64];
    g_snprintf(avg_waiting_time_str, sizeof(avg_waiting_time_str), "Average Waiting Time: %.2f", avg_waiting_time);
    strcat(result_message, "\n");
    strcat(result_message, avg_waiting_time_str);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, "%s", result_message);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_OK) {
        reset_processes();
    }
}

void sort_by_burst_time() {
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].burst_time > processes[j].burst_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void sjn_schedule() {
    sort_by_burst_time();

    int current_time = 0;
    int completed = 0;

    while (completed < num_processes) {
        int next_process = -1;
        int min_burst_time = INT_MAX;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                if (processes[i].burst_time < min_burst_time) {
                    min_burst_time = processes[i].burst_time;
                    next_process = i;
                }
            }
        }

        if (next_process != -1) {
            processes[next_process].completion_time = current_time + processes[next_process].burst_time;
            processes[next_process].waiting_time = current_time - processes[next_process].arrival_time; // Calculate waiting time
            current_time = processes[next_process].completion_time;
            completed++;
        } else {
            current_time++;
        }
    }

    display_scheduling_result("SJN Scheduling Complete");
}


void ljf_schedule() {
    int current_time = 0;
    int completed = 0;

    while (completed < num_processes) {
        int next_process = -1;
        int max_burst_time = INT_MIN;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                if (processes[i].burst_time > max_burst_time) {
                    max_burst_time = processes[i].burst_time;
                    next_process = i;
                }
            }
        }

        if (next_process != -1) {
            processes[next_process].completion_time = current_time + processes[next_process].burst_time;
            processes[next_process].waiting_time = current_time - processes[next_process].arrival_time; // Calculate waiting time
            current_time = processes[next_process].completion_time;
            completed++;
        } else {
            current_time++;
        }
    }

    display_scheduling_result("LJF Scheduling Complete");
}

void fcfs_schedule() {
    int current_time = 0;
    int completed = 0;

    // Sort processes by arrival time
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    while (completed < num_processes) {
        int next_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                next_process = i;
                break;
            }
        }

        if (next_process != -1) {
            processes[next_process].completion_time = current_time + processes[next_process].burst_time;
            processes[next_process].waiting_time = current_time - processes[next_process].arrival_time; // Calculate waiting time
            current_time = processes[next_process].completion_time;
            completed++;
        } else {
            // If no process is ready, move the current time to the arrival time of the next process
            int next_arrival_time = INT_MAX;
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].completion_time == 0 && processes[i].arrival_time < next_arrival_time) {
                    next_arrival_time = processes[i].arrival_time;
                }
            }
            current_time = next_arrival_time;
        }
    }

    display_scheduling_result("FCFS Scheduling Complete");
}

void priority_schedule() {
    int current_time = 0;
    int completed = 0;

    while (completed < num_processes) {
        int next_process = -1;
        int max_priority = INT_MIN;
        int earliest_arrival_time = INT_MAX;

        // Find the process with the highest priority among the arrived processes
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                if (processes[i].priority > max_priority || (processes[i].priority == max_priority && processes[i].arrival_time < earliest_arrival_time)) {
                    max_priority = processes[i].priority;
                    earliest_arrival_time = processes[i].arrival_time;
                    next_process = i;
                }
            }
        }

        if (next_process != -1) {
            // Calculate waiting time based on when the process starts execution
            processes[next_process].waiting_time = current_time - processes[next_process].arrival_time;
            if (processes[next_process].waiting_time < 0) {
                processes[next_process].waiting_time = 0; // Ensure waiting time is non-negative
            }

            // Update current time and completion time
            current_time += processes[next_process].burst_time;
            processes[next_process].completion_time = current_time;

            completed++;
        } else {
            current_time++;
        }
    }

    display_scheduling_result("Priority Scheduling Complete");
}


void round_robin_schedule(int time_quantum) {
    int current_time = 0;
    int remaining_burst[10];
    int waiting_time[10] = {0};  // Array to track waiting times for each process
    int arrival_time[10];        // Array to track arrival times for each process
    int finished = 0;            // Counter to track the number of finished processes

    for (int i = 0; i < num_processes; i++) {
        remaining_burst[i] = processes[i].burst_time;
        arrival_time[i] = processes[i].arrival_time;
    }

    while (finished < num_processes) {
        int all_done = 1;
        for (int i = 0; i < num_processes; i++) {
            if (remaining_burst[i] > 0 && arrival_time[i] <= current_time) {
                all_done = 0;
                if (remaining_burst[i] > time_quantum) {
                    waiting_time[i] += (current_time - processes[i].completion_time);
                    current_time += time_quantum;
                    remaining_burst[i] -= time_quantum;
                } else {
                    waiting_time[i] += (current_time - processes[i].completion_time);
                    current_time += remaining_burst[i];
                    remaining_burst[i] = 0;
                    processes[i].completion_time = current_time;
                    processes[i].waiting_time = waiting_time[i] + (current_time - arrival_time[i] - processes[i].burst_time);
                    finished++;
                }
            } else if (arrival_time[i] > current_time && all_done) {
                current_time = arrival_time[i];  // Advance time to the next process arrival if no process is ready
                all_done = 0;                   // Indicate that the current iteration is not actually done
            }
        }
    }

    display_scheduling_result("Round Robin Scheduling Complete");
}



void on_run_sjn_clicked(GtkButton *button, gpointer user_data) {
    sjn_schedule();
}

void on_run_fcfs_clicked(GtkButton *button, gpointer user_data) {
    fcfs_schedule();
}

void on_run_priority_clicked(GtkButton *button, gpointer user_data) {
    priority_schedule();
}

void on_run_round_robin_clicked(GtkButton *button, gpointer user_data) {
    int time_quantum = 1;
    round_robin_schedule(time_quantum);
}

void on_run_ljf_clicked(GtkButton *button, gpointer user_data) {
    ljf_schedule();
}

void on_run_clicked(GtkButton *button, gpointer user_data) {
    int burst_time = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(burst_entry));
    int arrival_time = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(arrival_entry));
    int priority = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(priority_entry));

    Process new_process;
    new_process.process_id = num_processes + 1;
    new_process.burst_time = burst_time;
    new_process.arrival_time = arrival_time;
    new_process.priority = priority;
    new_process.completion_time = 0;
    processes[num_processes++] = new_process;

    gchar process_details[128];
    g_snprintf(process_details, sizeof(process_details), "Process ID: %d\nArrival Time: %d\nBurst Time: %d\nPriority: %d", new_process.process_id, new_process.arrival_time, new_process.burst_time, new_process.priority);
    display_message_dialog(process_details);
}

void on_reset_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Reset processes and clear existing data?");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_OK) {
        clear_processes();
        display_message_dialog("Processes cleared. You can now add new processes.");
    }
}

void on_about_clicked(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Project by Kashan Mehmood and Husnain Nedeem");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *burst_label, *arrival_label, *priority_label;
    GtkWidget *burst_spin, *arrival_spin, *priority_spin;
    GtkWidget *add_process_button, *reset_button;
    GtkWidget *button_box;
    GtkWidget *run_sjn_button, *run_fcfs_button, *run_priority_button, *run_rr_button, *run_ljf_button;

    GtkWidget *menubar;
    GtkWidget *about_menu;
    GtkWidget *about_menu_item;

    burst_entry = burst_spin;
    arrival_entry = arrival_spin;
    priority_entry = priority_spin;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();
    about_menu = gtk_menu_new();

    about_menu_item = gtk_menu_item_new_with_label("About");
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), about_menu_item);
    g_signal_connect(about_menu_item, "activate", G_CALLBACK(on_about_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    label = gtk_label_new("Enter Process Arrival Time, Burst Time, and Priority:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    burst_label = gtk_label_new("Burst Time:");
    gtk_box_pack_start(GTK_BOX(vbox), burst_label, FALSE, FALSE, 0);

    burst_spin = gtk_spin_button_new_with_range(0, 100, 1);
    gtk_box_pack_start(GTK_BOX(vbox), burst_spin, FALSE, FALSE, 0);
    burst_entry = burst_spin;

    arrival_label = gtk_label_new("Arrival Time:");
    gtk_box_pack_start(GTK_BOX(vbox), arrival_label, FALSE, FALSE, 0);

    arrival_spin = gtk_spin_button_new_with_range(0, 100, 1);
    gtk_box_pack_start(GTK_BOX(vbox), arrival_spin, FALSE, FALSE, 0);
    arrival_entry = arrival_spin;

    priority_label = gtk_label_new("Priority:");
    gtk_box_pack_start(GTK_BOX(vbox), priority_label, FALSE, FALSE, 0);

    priority_spin = gtk_spin_button_new_with_range(0, 10, 1);
    gtk_box_pack_start(GTK_BOX(vbox), priority_spin, FALSE, FALSE, 0);
    priority_entry = priority_spin;

    add_process_button = gtk_button_new_with_label("Add Process");
    g_signal_connect(add_process_button, "clicked", G_CALLBACK(on_run_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), add_process_button, FALSE, FALSE, 0);

    reset_button = gtk_button_new_with_label("Reset Processes");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(on_reset_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), reset_button, FALSE, FALSE, 0);

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    run_sjn_button = gtk_button_new_with_label("Run SJN");
    g_signal_connect(run_sjn_button, "clicked", G_CALLBACK(on_run_sjn_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), run_sjn_button, FALSE, FALSE, 0);
    
    run_ljf_button = gtk_button_new_with_label("Run LJF");
    g_signal_connect(run_ljf_button, "clicked", G_CALLBACK(on_run_ljf_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), run_ljf_button, FALSE, FALSE, 0);

    run_fcfs_button = gtk_button_new_with_label("Run FCFS");
    g_signal_connect(run_fcfs_button, "clicked", G_CALLBACK(on_run_fcfs_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), run_fcfs_button, FALSE, FALSE, 0);

    run_priority_button = gtk_button_new_with_label("Run Priority");
    g_signal_connect(run_priority_button, "clicked", G_CALLBACK(on_run_priority_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), run_priority_button, FALSE, FALSE, 0);

    run_rr_button = gtk_button_new_with_label("Run Round Robin");
    g_signal_connect(run_rr_button, "clicked", G_CALLBACK(on_run_round_robin_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), run_rr_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}


