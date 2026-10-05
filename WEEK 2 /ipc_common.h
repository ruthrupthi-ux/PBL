#ifndef IPC_COMMON_H
#define IPC_COMMON_H

#include <mqueue.h>
#include <sys/types.h>

#define UI_TO_CORE_QUEUE   "/sim_ui_to_core"
#define CORE_TO_UI_QUEUE   "/sim_core_to_ui"
#define CORE_TO_LOG_QUEUE  "/sim_core_to_log"

#define IPC_TEXT_SIZE 256

typedef struct {
    int process_id;
    int burst_time;
    int priority;
    int command; /* 1=EXECUTE, 2=EXIT */
    char instruction[IPC_TEXT_SIZE];
} UIMessage;

typedef struct {
    int process_id;
    int status; /* 0=OK, 1=ERROR, 2=EXIT */
    char message[IPC_TEXT_SIZE];
} CoreResponse;

typedef struct {
    int process_id;
    int level; /* 0=INFO, 1=WARNING, 2=ERROR */
    char message[IPC_TEXT_SIZE];
} LogMessage;

#endif
