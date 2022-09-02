#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include "command_line.h"
#include "gas_cam_lib.h"
#include "hash_table.h"
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

void init_application();
gas_api* load_library();
void handle_sigint(int);

#endif // COMMAND_HANDLER_H
