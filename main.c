#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 2

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
    bool cpu_free        = true;

    // Main clock cycle loop
    while (clock_time < 200)
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
            fscanf(inf, "%s %d %d %d",
                temp.name,
                &temp.arrival_time,
                &temp.service_time,
                &temp.priority_level);
            temp.wait_time = 0;
            fgetc(inf);
        }

        // Check if CPU is free
        if (cpu_free && current_job < queue_size)
        {
            cpu_free = false;
        }

        // Increase wait time for every job in queue
        for (int i = current_job + 1; i < queue_size; i = i + 1)
        {
            job_queue[i].wait_time = job_queue[i].wait_time + 1;
        }

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
