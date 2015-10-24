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
    fscanf(inf, "%s %d %d %d\n",
        temp.name,
        &temp.arrival_time,
        &temp.service_time,
        &temp.priority_level);
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
            realloc(job_queue, sizeof(JOB));
            queue_size = queue_size + 1;
            fscanf(inf, "%s %d %d %d\n",
                temp.name,
                &temp.arrival_time,
                &temp.service_time,
                &temp.priority_level);
            temp.wait_time = 0;
        }

        // Check if CPU is free
        if (cpu_free)
        {
            current_job = current_job + 1;
            cpu_free = false;
        }

        // Increase wait time for every job in queue
        for (int i = 0; i < queue_size; i = i + 1)
        {
            if (current_job != i)
                job_queue[i].wait_time = job_queue[i].wait_time + 1;
        }

        // increase clock cycle
        clock_time = clock_time + 1;
    }

    free(job_queue);
}