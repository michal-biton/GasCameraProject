#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include "gas_cam_lib.h"
#include "command_handler.h"
#include "rpc_protocol.h"
#include "config.h"
#include <ctype.h>
#define MAX_DATA_SIZE 1024

int GAS_API_START_RECORD_WRAPER();
int GAS_API_STOP_RECORD_WRAPER();
int GAS_API_DO_SNAPSHOT_WRAPER();
int GAS_API_GET_STATUS_WRAPER();
int GAS_API_GET_RECORD_PARAMS_WRAPER();
int GAS_API_SET_RECORD_PARAMS_WRAPER(char data[MAX_DATA_SIZE]);
int GAS_API_GET_SNAPSHOT_PARAMS_WRAPER();
int GAS_API_SET_SNAPSHOT_PARAMS_WRAPER(char data[MAX_DATA_SIZE]);

#endif // COMMAND_LINE_H
