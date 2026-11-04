#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include "core.h"
#include "ipc_common.h"

static int read_full(int fd, void *buffer, size_t size) {
    size_t total=0; char *p=(char *)buffer;
    while(total<size) {
        ssize_t n=read(fd,p+total,size-total);
        if(n==0) return 0;
        if(n<0) { if(errno==EINTR) continue; return -1; }
        total+=(size_t)n;
    }
    return 1;
}
static int write_full(int fd,const void *buffer,size_t size) {
    size_t total=0; const char *p=(const char *)buffer;
    while(total<size) {
        ssize_t n=write(fd,p+total,size-total);
        if(n<0) { if(errno==EINTR) continue; return 0; }
        if(n==0) return 0;
        total+=(size_t)n;
    }
    return 1;
}
static int execute_instruction(const char *instruction,CPU *cpu,Memory *memory,Stack *stack,Queue *queue,char *output,size_t output_size) {
    char operation[20];
    if(sscanf(instruction,"%19s",operation)!=1) { snprintf(output,output_size,"Empty instruction"); return 0; }
    if(!strcmp(operation,"LOAD")||!strcmp(operation,"ADD")||!strcmp(operation,"SUB")||!strcmp(operation,"MUL")||!strcmp(operation,"DIV")||!strcmp(operation,"PRINT")||!strcmp(operation,"HALT"))
        return cpu_execute(cpu,instruction,output,output_size);
    if(!strcmp(operation,"STORE")||!strcmp(operation,"READ"))
        return execute_memory(memory,instruction,output,output_size);
    if(!strcmp(operation,"PUSH")||!strcmp(operation,"POP")||!strcmp(operation,"SPEEK"))
        return execute_stack(stack,instruction,output,output_size);
    if(!strcmp(operation,"ENQUEUE")||!strcmp(operation,"DEQUEUE")||!strcmp(operation,"QPEEK"))
        return execute_queue(queue,instruction,output,output_size);
    snprintf(output,output_size,"Unknown instruction"); return 0;
}
int run_core(int ui_read_fd,int ui_write_fd,mqd_t logger_queue) {
    CPU cpu; Memory memory; Stack stack; Queue queue; UIMessage request;
    CoreResponse response; LogMessage log; char output[IPC_TEXT_SIZE]; int rr;
    cpu_init(&cpu); memory_init(&memory); stack_init(&stack); queue_init(&queue);
    for(;;) {
        rr=read_full(ui_read_fd,&request,sizeof(request));
        if(rr<=0) break;
        request.instruction[IPC_TEXT_SIZE-1] = '\0';
        memset(&response,0,sizeof(response)); memset(&log,0,sizeof(log)); memset(output,0,sizeof(output));
        response.process_id=request.process_id; log.process_id=request.process_id;
        response.status=execute_instruction(request.instruction,&cpu,&memory,&stack,&queue,output,sizeof(output));
        snprintf(response.message,sizeof(response.message),"%s",output);
        log.level=response.status?1:2;
        snprintf(log.message,sizeof(log.message),"Instruction: %.100s | Result: %.120s",request.instruction,output);
        if(!write_full(ui_write_fd,&response,sizeof(response))) break;
        if (mq_send(logger_queue, (const char *)&log, sizeof(log), 0) == -1) {
            perror("Core: mq_send");
            break;
        }
    }
    memset(&log, 0, sizeof(log));
    log.level = 1;
    snprintf(log.message, sizeof(log.message), "Core process received shutdown request.");
    if (mq_send(logger_queue, (const char *)&log, sizeof(log), 0) == -1)
        perror("Core: shutdown mq_send");
    close(ui_read_fd);
    close(ui_write_fd);
    return 0;
}

