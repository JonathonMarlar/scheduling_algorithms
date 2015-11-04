#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 2
#define CLOCK_MAX 200
#define TIME_QUANTUM 10

typedef struct _job
{
    char name[NAME_LENGTH];
    int arrival_time;
    int service_time;
    int priority_level;
    int wait_time;
} JOB;

int main(void)
{
    // Initialization
    FILE *inf            = fopen("in.txt", "r");
    JOB  temp;
    fscanf(inf, "%s %d %d %d",
        temp.name,
        &temp.arrival_time,
        &temp.service_time,
        &temp.priority_level);
    fgetc(inf);
    temp.wait_time = 0;
    JOB  *job_queue      = malloc(sizeof(JOB));
    int  queue_size      = 0;
    int  current_job     = 0;
    int  clock_time      = 0;
    int  time_q          = TIME_QUANTUM;
    bool cpu_free        = true;

    // Main clock cycle loop
    while (clock_time < CLOCK_MAX)
    {
        // Check job completion in CPU
        if (cpu_free == false)
        {
            // Decrease service time of current job
            job_queue[current_job].service_time = job_queue[current_job].service_time - 1;
            if (job_queue[current_job].service_time == 0)
            {
                printf("Job complete!\n \
                    Job Name: %s\n \
                    Arrival Time: %d\n \
                    Wait Time: %d\n \
                    Clock Time of Completion: %d\n",
                    job_queue[current_job].name,
                    job_queue[current_job].arrival_time,
                    job_queue[current_job].wait_time,
                    clock_time);
                current_job = current_job + 1;
                if (current_job >= queue_size)
                    current_job = 0;
                time_q = TIME_QUANTUM;
                cpu_free = true;
            }
            // decrease time quantum
            time_q = time_q - 1;
            if (time_q == 0)
            {
                current_job = current_job + 1;
                if (current_job >= queue_size)
                    current_job = 0;
                time_q = TIME_QUANTUM;
                cpu_free = true;
            }
        }

        // Check for arrivals in the waiting queue
        while (temp.arrival_time == clock_time)
        {
            strcpy(job_queue[queue_size].name, temp.name);
            job_queue[queue_size].arrival_time = temp.arrival_time;
            job_queue[queue_size].service_time = temp.service_time;
            job_queue[queue_size].priority_level = temp.priority_level;
            job_queue[queue_size].wait_time = temp.wait_time;
            queue_size = queue_size + 1;
            job_queue = realloc(job_queue, (queue_size + 1) * sizeof(JOB));
            if (fscanf(inf, "%s %d %d %d",
                temp.name,
                &temp.arrival_time,
                &temp.service_time,
                &temp.priority_level) != EOF)
            {
                temp.wait_time = 0;
            }
            else
                {
                    // it's EOF, so destroy temp
                    temp.arrival_time = 0;
                    temp.service_time = 0;
                    temp.priority_level = 0;
                }
            fgetc(inf);
        }

        // Check if CPU is free
        int c = current_job + 1;
        if (c > queue_size)
            c = 0;
        while (job_queue[c].service_time == 0)
        {
            c = c + 1;
            if (c > queue_size)
                c = 0;
            if (c == current_job)
                break;
            if (job_queue[c].service_time != 0)
                current_job = c;
        }
        if (cpu_free && current_job < queue_size && job_queue[current_job].service_time != 0)
        {
            cpu_free = false;
        }

        // Increase wait time for every job in queue
        for (int i = 0; i < queue_size; i = i + 1)
        {
            if (i != current_job && job_queue[i].service_time != 0)
                job_queue[i].wait_time = job_queue[i].wait_time + 1;
        }

        // increase clock cycle
        clock_time = clock_time + 1;
    }


    // Debug
    printf("Clock Time: %d\n"
            "Current Job: %d\n"
            "Time Quantum: %d\n"
            "Queue Size: %d\n\n",
            clock_time, current_job, time_q, queue_size);
    for (int i = 0; i < queue_size; i = i + 1)
    {
        printf("Name: %s\n"
            "Service Time: %d\n\n",
            job_queue[i].name, job_queue[i].service_time);
    }
    free(job_queue);

    return 0;
}
