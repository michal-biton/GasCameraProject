#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include "command_line.h"
#include <pthread.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#define UPDATETHREADWAIT 4
#define MAXPARAMETERS 100
#define MAGIC 0x10203040
#define CAPACITY 500 // Size of the Hash Table
#include <assert.h>

enum RESULT_E{
    ERROR,
    SUCCESS
};

enum function_name{
    START_RECORD,
    STOP_RECORD,
    DO_SNAPSHOT,
    GET_RECORD_PARAMETERS,
    SET_RECORD_PARAMETERS,
    GET_SNAPSHOT_PARAMETERS,
    SET_SNAPSHOT_PARAMETERS
};

char** split(char*);
void handle_sigint(int);
void* cfg_update(void*);

#endif // COMMAND_HANDLER_H
