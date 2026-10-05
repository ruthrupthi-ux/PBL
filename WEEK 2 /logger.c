#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <mqueue.h>
#include "ipc_common.h"

#define LOG_DIR "logs"
#define LOG_FILE "logs/simulator.log"

static void write_log(const char *level, const char *message)
{
    FILE *file;
    time_t now;
    struct tm *local_time;
    char timestamp[32];

    mkdir(LOG_DIR, 0755);

    file = fopen(LOG_FILE, "a");
    if (file == NULL) {
        perror("LOGGER: Unable to open log file");
        return;
    }

    now = time(NULL);
    local_time = localtime(&now);

    if (local_time != NULL)
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);
    else
        snprintf(timestamp, sizeof(timestamp), "unknown-time");

    fprintf(file, "%s | %s | %s\n", timestamp, level, message);
    fclose(file);
}

int main(void)
{
    mqd_t from_core = mq_open(CORE_TO_LOG_QUEUE, O_RDONLY);
    if (from_core == (mqd_t)-1) {
        perror("LOGGER: unable to open IPC queue");
        return 1;
    }

    printf("LOGGER PROCESS STARTED\n");
    printf("Waiting for events from Core...\n");

    LogMessage log;

    while (1) {
        if (mq_receive(from_core, (char *)&log, sizeof(log), NULL) == -1) {
            perror("LOGGER: mq_receive");
            break;
        }

        const char *level = "INFO";
        if (log.level == 1) level = "WARNING";
        else if (log.level == 2) level = "ERROR";

        char message[IPC_TEXT_SIZE + 64];
        snprintf(message, sizeof(message), "Process %d: %s",
                 log.process_id, log.message);

        write_log(level, message);
        printf("[LOGGER] %s | %s\n", level, message);

        if (strstr(log.message, "shutdown request") != NULL)
            break;
    }

    mq_close(from_core);
    return 0;
}
