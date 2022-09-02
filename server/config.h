#ifndef CONFIG_H
#define CONFIG_H
#pragma once
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gas_cam_lib.h"
#define JSONFILEPATH "/mnt/hgfs/GasCameraProject/json.txt"
#define DECLARATIONSIZE 500

typedef struct{
    char* debug;
    record_t* record;
    snapshot_t* snapshot;
    streaming_t* streaming;
    gpio_t* gpio;
}WIDGET,*PWIDGET;

typedef struct{
    WIDGET* widget;
    FILE* json_file;
}CFG,*PCFG;

char** cfg_split(char*, int, char*);
char* create_buffer(WIDGET*);
void write_file(CFG*);
void read_file(CFG*);

//per object
void write_record(PWIDGET, record_t*);
void write_snapshot(PWIDGET, snapshot_t*);
void write_streaming(PWIDGET, streaming_t*);
void write_gpio(PWIDGET, gpio_t*);
void read_record(PWIDGET, char*);
void read_snapshot(PWIDGET, char*);
void read_streaming(PWIDGET, char*);
void read_gpio(PWIDGET, char*);
char* create_buffer_record(record_t*);
char* create_buffer_snapshot(snapshot_t*);
char* create_buffer_streaming(streaming_t*);
char* create_buffer_gpio(gpio_t*);

#endif // CONFIG_H
