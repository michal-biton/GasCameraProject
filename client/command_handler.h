#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include<string.h>
#include"private_command_line.h"
#include<stdio.h>
#include<hash_table.h>
#include <dlfcn.h>
#include<stdlib.h>
#include<assert.h>
#define SIGINT 2
void init_hash_table(HashTable*);
void decoderfunction(HashTable*,char**);
char** split(char*);
void handle_sigint(int sig);

#endif // COMMAND_HANDLER_H
