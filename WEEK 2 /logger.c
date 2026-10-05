#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#define LOG_DIR "logs"
#define LOG_FILE "logs/simulator.log"

static void write_log(const char *level, const char *message) {
    FILE *file;
    time_t now;
    struct tm *local_time;
    char timestamp[32];

    mkdir(LOG_DIR, 0755);

    file = fopen(LOG_FILE, "a");
    if (file == NULL) {
        perror("Unable to open log file");
        return;
    }

    now = time(NULL);
    local_time = localtime(&now);

    if (local_time != NULL) {
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
                 local_time);
    } else {
        snprintf(timestamp, sizeof(timestamp), "unknown-time");
    }

    fprintf(file, "%s | %s | %s\n", timestamp, level, message);
    fclose(file);
}

void log_info(const char *message) {
    write_log("INFO", message);
}

void log_warning(const char *message) {
    write_log("WARNING", message);
}

void log_error(const char *message) {
    write_log("ERROR", message);
}

int main(void) {
    log_info("Logger started successfully.");
    log_info("Sample execution event recorded.");
    log_warning("Sample warning recorded.");
    log_error("Sample error recorded.");

    printf("Sample logs written to %s\n", LOG_FILE);
    return 0;
}
