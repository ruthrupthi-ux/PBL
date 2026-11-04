#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>
#include "ipc_common.h"

static void cleanup_ipc(void)
{
    mq_unlink(CORE_TO_LOG_QUEUE);
    unlink(UI_TO_CORE_FIFO);
    unlink(CORE_TO_UI_FIFO);
}

static pid_t start_process(const char *path)
{
    pid_t pid = fork();
    if (pid == 0) {
        execl(path, path, (char *)NULL);
        perror(path);
        _exit(127);
    }
    return pid;
}

int main(void)
{
    struct mq_attr attr = {0};
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(LogMessage);

    cleanup_ipc();
    mqd_t q = mq_open(CORE_TO_LOG_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (q == (mqd_t)-1) {
        perror("Launcher: mq_open");
        return EXIT_FAILURE;
    }
    mq_close(q);

    pid_t logger_pid = start_process("./logger");
    if (logger_pid < 0) { perror("fork logger"); cleanup_ipc(); return 1; }
    pid_t core_pid = start_process("./core_process");
    if (core_pid < 0) { perror("fork core"); kill(logger_pid, SIGTERM); waitpid(logger_pid,NULL,0); cleanup_ipc(); return 1; }
    pid_t ui_pid = start_process("./UI");
    if (ui_pid < 0) { perror("fork UI"); kill(core_pid,SIGTERM); kill(logger_pid,SIGTERM); waitpid(core_pid,NULL,0); waitpid(logger_pid,NULL,0); cleanup_ipc(); return 1; }

    printf("========================================\n");
    printf(" MULTI-PROCESS SIMULATOR STARTED\n");
    printf(" UI PID     : %d\n CORE PID   : %d\n LOGGER PID : %d\n", ui_pid, core_pid, logger_pid);
    printf("========================================\n");

    int status;
    waitpid(ui_pid, &status, 0);
    /* UI closing its FIFO sends EOF to Core; Core then tells Logger to stop. */
    waitpid(core_pid, NULL, 0);
    waitpid(logger_pid, NULL, 0);
    cleanup_ipc();
    printf("All processes stopped. IPC resources cleaned up.\n");
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}
