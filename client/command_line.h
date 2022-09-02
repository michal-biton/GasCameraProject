#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <ctype.h>
#include "command_handler.h"
#include "rpc_protocol.h"

typedef struct{
    char* file_name;
    int codec;
    int width;
    int height;
    int fps;
}record_t;

typedef struct snapshot_t{
    char* file_name;
    int width;
    int height;
    int type;
}snapshot_t;

int GAS_API_START_RECORD_CLI();
int GAS_API_STOP_RECORD_CLI();
int GAS_API_DO_SNAPSHOT_CLI();
int GAS_API_GET_STATUS_CLI();
int GAS_API_GET_RECORD_PARAMS();
int GAS_API_SET_RECORD_PARAMS(char**);
int GAS_API_GET_SNAPSHOT_PARAMS();
int GAS_API_SET_SNAPSHOT_PARAMS(char**);

#endif // COMMAND_LINE_H
