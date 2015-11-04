#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 2
#define CLOCK_TIME 300
#define QUANTUM_T 10

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
    int  time_q          = QUANTUM_T;
    bool cpu_free        = true;

    // Main clock cycle loop
    while (clock_time < CLOCK_TIME)
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
                {
                    current_job = 0;
                }
                cpu_free = true;
            }

            // decrease the time quantum
            time_q = time_q - 1;
            if (time_q == 0)
            {
                // free the CPU
                current_job = current_job + 1;
                if (current_job >= queue_size)
                {
                    current_job = 0;
                }
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
        if (cpu_free && current_job < queue_size && job_queue[current_job].service_time != 0)
        {
            cpu_free = false;
        }
        else if (job_queue[current_job].service_time == 0)
        {
            int c = current_job;
            while (job_queue[c].service_time == 0)
            {
                c = c + 1;
                if (c > queue_size)
                    c = 0;
                // if we've looped around once and there's no jobs, quit
                if (c == current_job)
                    break;
            }
            current_job = c;
        }

        // Increase wait time for every job in queue
        for (int i = 0; i < queue_size; i = i + 1)
        {
            if (i != current_job && job_queue[i].service_time != 0)
                job_queue[i].wait_time = job_queue[i].wait_time + 1;
        }
        //printf("Current Job: %d\n\n", current_job);
        /*
        // debug
        printf("Current Job: %d, Queue Size: %d, Clock Time: %d\n\n", current_job, queue_size, clock_time);
        for (int i = 0; i < queue_size; i++)
        {
            printf("Job Name: %s,\n Arrival time: %d,\n service time: %d,\n priority: %d,\n wait time: %d\n\n",
                job_queue[i].name, job_queue[i].arrival_time, job_queue[i].service_time, job_queue[i].priority_level, job_queue[i].wait_time);
        }
        */
        // increase clock cycle
        clock_time = clock_time + 1;
    }

    free(job_queue);

    return 0;
}

