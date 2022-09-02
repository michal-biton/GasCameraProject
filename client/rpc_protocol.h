#ifndef RPC_PROTOCOL_H
#define RPC_PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "command_handler.h"
#define PORT 8081
#define MAXLINE 1024
#define MAGIC 0x10203040
#define HEADER_SIZE 20
#define FOREVER 0

enum op_code{
    ACKNOWLEDGE = 1,
    RECORD = 10,
    SNAPSHOT = 21,
    GET_RECORD_PARAMS = 60,
    SET_RECORD_PARAMS = 61,
    GET_SNAPSHOT_PARAMS = 62,
    SET_SNAPSHOT_PARAMS = 63,
    GET_STATUS = 95,
    CONTROL = 100,
    GET_FILE_NAME_BY_INDEX = 80,
    DOWNLOAD_FILE = 81
};

enum type{
    OPERATION = 1,
    CONTROl = 2,
    GET_PARAMETERS = 3,
    SET_PARAMETERS = 4,
    GET_STATUS_TYPE = 5,
    ACKNOWLEDGE_TYPE = 6,
    REPLY_MESSAGE = 7,
};

enum control_data{
    REBOOT,
    SHUT_DOWN,
    RESET_SETTINGS
};

enum data0_acknowledge{
    ACK = 0,
    NACK = 1
};

enum data1_acknowledge{
    COMMAND_ERROR,
    INVALID_MASSAGE,
    FAILED
};

enum data_0_record{
    STOP,
    START,
    FORCE_STATR
};

enum snapshot_deta{
    BMP,
    JPEG
};

enum error_bits{
    STORAGE_ERROR,
    DISK_FULL
};

enum STATUS_BITS{
    RECORD_BIT,
    SNAPSHOP_BIT
};

typedef struct{
    unsigned int magic;
    uint8_t header_size;
    uint8_t type;
    uint16_t op_code;
    int data_size;
    uint8_t checksum;
    char reserve[7];
    char data[100];
}command_massage;

int init_socket();
int communicate_with_server(char*);
int create_request(char*, uint8_t, uint16_t, int);

#endif // RPC_PROTOCOL_H
