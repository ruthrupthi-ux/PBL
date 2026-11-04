#ifndef IPC_COMMON_H
#define IPC_COMMON_H

#define IPC_TEXT_SIZE 256

#define UI_TO_CORE_FIFO "/tmp/ui_to_core_fifo"
#define CORE_TO_UI_FIFO "/tmp/core_to_ui_fifo"
#define CORE_TO_LOG_QUEUE "/core_to_log_queue"

typedef struct {
    int command;
    int process_id;
    char instruction[IPC_TEXT_SIZE];
} UIMessage;

typedef struct {
    int process_id;
    int status;
    char message[IPC_TEXT_SIZE];
} CoreResponse;

typedef struct {
    int process_id;
    int level;
    char message[IPC_TEXT_SIZE];
} LogMessage;

#endif
