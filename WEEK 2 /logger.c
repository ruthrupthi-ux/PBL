#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <time.h>
#include "ipc_common.h"

void get_time(char *buffer, int size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void write_log(const char *filename, const char *message)
{
    FILE *file = fopen(filename, "a");

    if (file == NULL)
    {
        perror("Logger: unable to open log file");
        return;
    }

    char time_buffer[30];

    get_time(time_buffer, sizeof(time_buffer));

    fprintf(file, "[%s] %s\n", time_buffer, message);

    fclose(file);
}

int main(void)
{
    mqd_t log_queue;
    LogMessage log;

    printf("========================================\n");
    printf("     STUDENT 3: LOGGING PROCESS\n");
    printf("========================================\n");

    /* Open Core-to-Logger POSIX message queue */
    log_queue = mq_open(CORE_TO_LOG_QUEUE, O_RDONLY);

    if (log_queue == (mqd_t)-1)
    {
        perror("Logger: unable to open message queue");
        return 1;
    }

    printf("Logger waiting for messages from Core...\n");

    while (1)
    {
        /* Receive log message from Core */
        if (mq_receive(log_queue,
                       (char *)&log,
                       sizeof(log),
                       NULL) == -1)
        {
            perror("Logger: mq_receive");
            break;
        }

        printf("Logger received: %s\n", log.message);

        /* ERROR messages go to error.log */
        if (log.level == 2)
        {
            write_log("error.log", log.message);
        }
        else
        {
            /* Normal execution messages go to execution.log */
            write_log("execution.log", log.message);
        }

        /*
         * Core sends this message when the simulator
         * is shutting down.
         */
        if (strcmp(log.message,
                   "Core process received shutdown request.") == 0)
        {
            printf("Logging process stopping...\n");
            break;
        }
    }

    mq_close(log_queue);

    printf("Logs saved successfully.\n");

    return 0;
}
