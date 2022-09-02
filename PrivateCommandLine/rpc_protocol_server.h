#ifndef RPC_PROTOCOL_SERVER_H
#define RPC_PROTOCOL_SERVER_H
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "gas_cam_lib.h"
#include"PrivateCommandLine.h"
#include "commandHandler.h"
#define PORT 8080
#define MAXLINE 1024
#define MAGIC 0x10203040
#define HEADER_SIZE 20
#define ERROR_NUMBER 0

enum op_code
{
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

enum data_0_acknowledge
{
    ACK = 0,
    NACK = 1
};

enum data_1_acknowledge
{
    COMMAND_ERROR = 0,
    INVALID_MASSAGE = 1,
    FAILED = 2
};

enum data_0_record
{
    STOP,
    START,
    FORCE_STATR
};

enum snapshot
{
    BMP,JPEG
};

enum control
{
    REBOOT,
    SHUT_DOWN,
    RESET_SETTINGS
};

enum type
{
    OPERATION = 1,
    CONTROl = 2,
    GET_PARAMETERS = 3,
    SET_PARAMETERS = 4,
    GET_STTAUS = 5,
    ACKNOWLEDGE_TYPE = 6,
    REPLY_MESSAGE = 7,
};

enum error_number_type
{
    NO_STORAGE = 1,
    NO_SPACE = 2,
    INVALID_FILE_NAME = 3
};

typedef struct
{
    unsigned int magic;
    uint8_t header_size;
    uint8_t type;
    uint16_t opcode;
    int data_size;
    uint8_t checksum;
    char reserve[7];
    char data[100];
}command_massage;

int init_socket();
int communicate_with_client(char*);
int read_massege(char*, command_massage*);
int act_command(command_massage*);
int recieve_request(char*);
int init_response();
char* acknowledge(uint8_t, uint8_t, uint8_t);

#endif // RPC_PROTOCOL_SERVER_H
