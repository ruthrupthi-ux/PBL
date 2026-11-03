#ifndef IPC_COMMON_H
#define IPC_COMMON_H

#define IPC_TEXT_SIZE 256

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
