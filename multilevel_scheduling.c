#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
typedef struct {
  int process_id;
  int burst_time;
  int priority;
  int remaining_time;
} Process;

// Function to sort the processes based on priority
void sort(Process *processes, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (processes[j].priority > processes[j + 1].priority) {
        Process temp = processes[j];
        processes[j] = processes[j + 1];
        processes[j + 1] = temp;
      }
    }
  }
}

// Function to execute processes in Queue 1 (Fixed Priority Preemptive) and create a Gantt chart
void execute_queue1(Process *processes, int n) {
  int time = 0;
  printf("Executing Queue 1 (Fixed Priority Preemptive):\n");
  printf("Queue 1 Gantt Chart: |");
  for (int i = 0; i < n; i++) {
    if (processes[i].priority == 0 && processes[i].remaining_time > 0) {
      printf("P%d|", processes[i].process_id);
      printf("Executing process %d from Queue 1\n", processes[i].process_id);
      // Ensure that the process time is strictly in multiples of 2
      int time_quantum1 = 2;
      if (processes[i].remaining_time <= time_quantum1) {
        processes[i].remaining_time = 0;
      } else {
        processes[i].remaining_time -= time_quantum1;
      }
    }
  }
  printf("\n");
}

// Function to execute processes in Queue 2 (Round Robin) and create a Gantt chart
void execute_queue2(Process *processes, int n, int time_quantum2) {
  int time = 0;
  printf("Executing Queue 2 (Round Robin):\n");
  printf("Queue 2 Gantt Chart: |");
  int remaining_processes = n;
  while (remaining_processes > 0) {
    for (int i = 0; i < n; i++) {
      if (processes[i].priority == 1 && processes[i].remaining_time > 0) {
        printf("P%d|", processes[i].process_id);
        printf("Executing process %d from Queue 2\n", processes[i].process_id);
        if (processes[i].remaining_time <= time_quantum2) {
          processes[i].remaining_time = 0;
          remaining_processes--;
        } else {
          processes[i].remaining_time -= time_quantum2;
        }
      }
    }
  }
  printf("\n");
}

int main() {
  int n; // Number of processes
  printf("Enter the number of processes: ");
  scanf("%d", &n);

  Process *processes = (Process *)malloc(n * sizeof(Process));

  // Input process details
  for (int i = 0; i < n; i++) {
    printf("Enter details for process %d:\n", i + 1);
    printf("Process ID: ");
    scanf("%d", &processes[i].process_id);
    printf("Burst Time: ");
    scanf("%d", &processes[i].burst_time);
    printf("Priority (0 for Queue 1, 1 for Queue 2): ");
    scanf("%d", &processes[i].priority);
    processes[i].remaining_time = processes[i].burst_time;
  }

  // Sort the processes based on priority
  sort(processes, n);

  // Implement multilevel scheduling
  execute_queue1(processes, n);
  execute_queue2(processes, n, 3); // You can adjust the time quantum for Queue 2 here

  // Calculate and print turnaround_time and waiting_time
  int *turnaround_time = (int *)malloc(n * sizeof(int));
  int *waiting_time = (int *)malloc(n * sizeof(int));

  for (int i = 0; i < n; i++) {
    turnaround_time[i] = 0;
    waiting_time[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    int process_id = processes[i].process_id;
    for (int j = 0; j < n; j++) {
      if (processes[j].process_id == process_id) {
        if (processes[j].priority == 0) {
          turnaround_time[i] += processes[j].burst_time;
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    int process_id = processes[i].process_id;
    for (int j = 0; j < n; j++) {
      if (processes[j].process_id == process_id && processes[j].priority == 1) {
        waiting_time[i] += turnaround_time[i] - processes[j].burst_time;
      }
    }
  }

  // Print turnaround_time and waiting_time
  printf("Process\tTurnaround Time\tWaiting Time\n");
  for (int i = 0; i < n; i++) {
    printf("P%d\t%d\t\t%d\n", processes[i].process_id, turnaround_time[i], waiting_time[i]);
  }

  free(processes);
  free(turnaround_time);
  free(waiting_time);

  return 0;
}

