#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>
#include "ipc_common.h"

static void cleanup_queues(void)
{
    mq_unlink(UI_TO_CORE_QUEUE);
    mq_unlink(CORE_TO_UI_QUEUE);
    mq_unlink(CORE_TO_LOG_QUEUE);
}

int main(void)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(UIMessage);
    attr.mq_curmsgs = 0;

    cleanup_queues();

    mqd_t q1 = mq_open(UI_TO_CORE_QUEUE, O_CREAT | O_RDWR, 0666, &attr);

    attr.mq_msgsize = sizeof(CoreResponse);
    mqd_t q2 = mq_open(CORE_TO_UI_QUEUE, O_CREAT | O_RDWR, 0666, &attr);

    attr.mq_msgsize = sizeof(LogMessage);
    mqd_t q3 = mq_open(CORE_TO_LOG_QUEUE, O_CREAT | O_RDWR, 0666, &attr);

    if (q1 == (mqd_t)-1 || q2 == (mqd_t)-1 || q3 == (mqd_t)-1) {
        perror("Launcher: unable to create message queues");
        cleanup_queues();
        return 1;
    }

    mq_close(q1);
    mq_close(q2);
    mq_close(q3);

    pid_t logger_pid = fork();
    if (logger_pid == 0) {
        execl("./logger", "./logger", (char *)NULL);
        perror("Launcher: execl logger");
        exit(1);
    }

    pid_t core_pid = fork();
    if (core_pid == 0) {
        execl("./core_process", "./core_process", (char *)NULL);
perror("Launcher: execl core_process");
        exit(1);
    }

    pid_t ui_pid = fork();
    if (ui_pid == 0) {
        execl("./UI", "./UI", (char *)NULL);
        perror("Launcher: execl UI");
        exit(1);
    }

    if (logger_pid < 0 || core_pid < 0 || ui_pid < 0) {
        perror("Launcher: fork");
        cleanup_queues();
        return 1;
    }

    printf("========================================\n");
    printf(" MULTI-PROCESS SIMULATOR STARTED\n");
    printf(" UI PID     : %d\n", ui_pid);
    printf(" CORE PID   : %d\n", core_pid);
    printf(" LOGGER PID : %d\n", logger_pid);
    printf("========================================\n");

    waitpid(ui_pid, NULL, 0);
    waitpid(core_pid, NULL, 0);
    waitpid(logger_pid, NULL, 0);

    cleanup_queues();
    printf("All processes stopped. IPC queues cleaned up.\n");
    return 0;
}
