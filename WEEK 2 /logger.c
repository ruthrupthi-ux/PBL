



#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include "ipc_common.h"


/* Get current date and time */
void get_time(char *buffer, int size)
{
    time_t now = time(NULL);
    struct tm time_info;

    if (localtime_r(&now, &time_info) == NULL)
    {
        snprintf(buffer, size, "Unknown time");
        return;
    }

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S",
             &time_info);
}

/* Write messages into log files */
void write_log(const char *filename, const char *message)
{
    FILE *file;
    char time_buffer[30];

    file = fopen(filename, "a");

    if (file == NULL)
    {
        perror("Logger: unable to open log file");
        return;
    }

    get_time(time_buffer, sizeof(time_buffer));

    fprintf(file, "[%s] %s\n", time_buffer, message);

    fclose(file);
}

/* Main Logger Process */
int main(void)
{
    mqd_t log_queue;
    LogMessage log;
    ssize_t bytes_received;

    printf("========================================\n");
    printf("       STUDENT 3: LOGGING PROCESS\n");
    printf("========================================\n");

    /* Open Core-to-Logger POSIX message queue */
    log_queue = mq_open(CORE_TO_LOG_QUEUE, O_RDONLY);

    if (log_queue == (mqd_t)-1)
    {
        perror("Logger: unable to open message queue");
        return EXIT_FAILURE;
    }

    printf("Logger connected to message queue.\n");
    printf("Logger waiting for messages from Core...\n");

    while (1)
    {
        memset(&log, 0, sizeof(LogMessage));

        /* Receive message from Core */
        bytes_received = mq_receive(
            log_queue,
            (char *)&log,
            sizeof(LogMessage),
            NULL
        );

        if (bytes_received == -1)
        {
            if (errno == EINTR)
                continue;

            perror("Logger: mq_receive");
            break;
        }

        /* Validate received message size */
        if (bytes_received != sizeof(LogMessage))
        {
            fprintf(stderr,
                    "Logger: invalid message size received.\n");
            continue;
        }

        /* Ensure message is null-terminated */
        log.message[IPC_TEXT_SIZE - 1] = '\0';

        printf("Logger received [PID: %d]: %s\n",
               log.process_id, log.message);

        /* ERROR messages go to error.log */
        if (log.level == 2)
        {
            write_log("error.log", log.message);
        }
        else
        {
            /* Normal messages go to execution.log */
            write_log("execution.log", log.message);
        }

        /* Check shutdown request */
        if (strcmp(log.message,
                   "Core process received shutdown request.") == 0)
        {
            printf("Logging process stopping...\n");
            break;
        }
    }

    /* Close message queue */
    if (mq_close(log_queue) == -1)
    {
        perror("Logger: mq_close");
        return EXIT_FAILURE;
    }

    printf("Logs saved successfully.\n");

    return EXIT_SUCCESS;
}
