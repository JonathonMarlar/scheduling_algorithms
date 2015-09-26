/**
* First Come First Serve (FCFS) Scheduling Algorithm
* pseudocode.cpp
*
* VERY EARLY pseudocode. Don't take any of this as the final code.
* Or good code. Avert your eyes. Marked as a .cpp only for markup.
**/

int main()
{
    // grab the list of jobs from a file
    list<job> = getJobList();

    // initialize the clock
    int clocktime = 0;

    // initialize job completion bool
    // starting out, there's no job in the CPU
    bool job_in_cpu = false;

    job current_job = NULL;

    // main cycle loop
    while (clocktime != 100)
    {
        // Check if there's a job in the CPU
        if (job_in_cpu)
        {
            current_job.service_time--;
            if (current_job.service_time == 0)
            {
                // print job complete
                printf("Job n completed on clock %n", clocktime);
                job_in_cpu = false;
            }
        }

        // check to see if any job gets in the queue
        for (int i = 0; i < list<job>.size; i++)
        {
            if (list<job>[i].arrival_time == clocktime)
            {
                inQueue(list<job>[i]);
            }
        }

        // if the CPU's free, put a job in!
        if (!job_in_cpu)
        {
            current_job = deQueue();
        }

        // increment clock
        clocktime++;
    }
}