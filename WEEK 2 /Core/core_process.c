#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include "core.h"
#include "ipc_common.h"

int main(void)
{
    int ui_read_fd, ui_write_fd;
    mqd_t log_queue;

    if (mkfifo(UI_TO_CORE_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("Core: mkfifo UI_TO_CORE_FIFO");
        return 1;
    }
    if (mkfifo(CORE_TO_UI_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("Core: mkfifo CORE_TO_UI_FIFO");
        return 1;
    }

    log_queue = mq_open(CORE_TO_LOG_QUEUE, O_WRONLY);
    if (log_queue == (mqd_t)-1) {
        perror("Core: mq_open logger queue");
        return 1;
    }

    /* These opens pair with UI's O_WRONLY then O_RDONLY opens. */
    ui_read_fd = open(UI_TO_CORE_FIFO, O_RDONLY);
    if (ui_read_fd == -1) {
        perror("Core: open UI FIFO");
        mq_close(log_queue);
        return 1;
    }
    ui_write_fd = open(CORE_TO_UI_FIFO, O_WRONLY);
    if (ui_write_fd == -1) {
        perror("Core: open response FIFO");
        close(ui_read_fd);
        mq_close(log_queue);
        return 1;
    }

    printf("Core process connected to UI and Logger.\n");
    run_core(ui_read_fd, ui_write_fd, log_queue);
    mq_close(log_queue);
    return 0;
}
