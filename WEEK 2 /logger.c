#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/simulator_log_fifo"
#define EXECUTION_LOG "execution.log"
#define ERROR_LOG "error.log"
#define BUFFER_SIZE 1024

static volatile sig_atomic_t running = 1;

static void handle_signal(int signal_number)
{
    (void)signal_number;
    running = 0;
}

static void get_timestamp(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm local_time;

    localtime_r(&now, &local_time);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", &local_time);
}

static void write_log(const char *level, const char *message)
{
    char timestamp[32];
    const char *filename = EXECUTION_LOG;

    get_timestamp(timestamp, sizeof(timestamp));

    if (strcmp(level, "ERROR") == 0)
        filename = ERROR_LOG;

    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        perror("Logger: fopen");
        return;
    }

    fprintf(file, "[%s] [%s] %s\n", timestamp, level, message);
    fclose(file);

    /* Also display the received log on the terminal. */
    printf("[%s] [%s] %s\n", timestamp, level, message);
    fflush(stdout);
}

static void process_message(char *message)
{
    /* Remove newline characters. */
    message[strcspn(message, "\r\n")] = '\0';

    if (message[0] == '\0')
        return;

    char *separator = strchr(message, '|');

    if (separator == NULL)
    {
        write_log("INFO", message);
        return;
    }

    *separator = '\0';

    const char *level = message;
    const char *text = separator + 1;

    if (strcmp(level, "INFO") != 0 &&
        strcmp(level, "ERROR") != 0 &&
        strcmp(level, "WARNING") != 0)
    {
        level = "INFO";
    }

    write_log(level, text);
}

int main(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /*
     * Create the FIFO if it does not already exist.
     * 0666 allows the user/processes to read and write according to
     * the system's umask.
     */
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST)
    {
        perror("Logger: mkfifo");
        return EXIT_FAILURE;
    }

    printf("========================================\n");
    printf(" Student 3 - Logging Process\n");
    printf(" FIFO: %s\n", FIFO_PATH);
    printf(" Waiting for log messages...\n");
    printf(" Press Ctrl+C to stop.\n");
    printf("========================================\n");

    while (running)
    {
        /*
         * Opening the FIFO for reading blocks until another process opens
         * it for writing. This keeps the logger as an independent process.
         */
        int fd = open(FIFO_PATH, O_RDONLY);

        if (fd == -1)
        {
            if (errno == EINTR && !running)
                break;

            perror("Logger: open FIFO");
            continue;
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        size_t message_length = 0;

        while (running &&
               (bytes_read = read(fd, buffer + message_length,
                                  sizeof(buffer) - message_length - 1)) > 0)
        {
            message_length += (size_t)bytes_read;
            buffer[message_length] = '\0';

            char *start = buffer;
            char *newline;

            while ((newline = strchr(start, '\n')) != NULL)
            {
                *newline = '\0';
                process_message(start);
                start = newline + 1;
            }

            /*
             * Keep an incomplete final message for the next read.
             */
            if (start != buffer)
            {
                size_t remaining = strlen(start);
                memmove(buffer, start, remaining);
                message_length = remaining;
                buffer[message_length] = '\0';
            }

            if (message_length == sizeof(buffer) - 1)
            {
                /* Prevent an oversized message from filling the buffer. */
                process_message(buffer);
                message_length = 0;
                buffer[0] = '\0';
            }
        }

        close(fd);

        if (bytes_read == -1 && errno != EINTR)
            perror("Logger: read FIFO");
    }

    unlink(FIFO_PATH);
    printf("\nLogger stopped.\n");

    return EXIT_SUCCESS;
}
