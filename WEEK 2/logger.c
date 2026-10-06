/*
 * Student 3 - Logging Process
 * Multi-Process Simulator & IPC
 *
 * Responsibility:
 * - Receive messages from another process using POSIX FIFO
 * - Store execution messages in execution.log
 * - Store ERROR messages in error.log
 * - Display received messages on the screen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define FIFO_NAME "simulator_log_fifo"
#define BUFFER_SIZE 512

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
        perror("Unable to open log file");
        return;
    }

    char time_buffer[30];
    get_time(time_buffer, sizeof(time_buffer));

    fprintf(file, "[%s] %s\n", time_buffer, message);
    fclose(file);
}

int main()
{
    int fd;
    char buffer[BUFFER_SIZE];

    printf("=== Student 3: Logging Process ===\n");

    /* Create FIFO if it does not already exist */
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        return 1;
    }

    printf("Waiting for messages from Core Process...\n");

    /* Open FIFO for reading */
    fd = open(FIFO_NAME, O_RDONLY);

    if (fd == -1)
    {
        perror("FIFO open");
        return 1;
    }

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';

            printf("Received: %s", buffer);

            /*
             * Messages beginning with ERROR are stored
             * in the separate error log.
             */
            if (strncmp(buffer, "ERROR:", 6) == 0)
            {
                write_log("error.log", buffer);
            }
            else
            {
                write_log("execution.log", buffer);
            }

            /*
             * The Core/UI process can send EXIT
             * when the simulator is finished.
             */
            if (strncmp(buffer, "EXIT", 4) == 0)
            {
                printf("Logging process stopping...\n");
                break;
            }
        }
        else if (bytes_read == 0)
        {
            /*
             * Writer closed the FIFO.
             * Reopen it so the logger can receive future messages.
             */
            close(fd);
            fd = open(FIFO_NAME, O_RDONLY);

            if (fd == -1)
            {
                perror("FIFO reopen");
                break;
            }
        }
        else
        {
            perror("read");
            break;
        }
    }

    close(fd);
    unlink(FIFO_NAME);

    printf("Logs saved successfully.\n");

    return 0;
}
