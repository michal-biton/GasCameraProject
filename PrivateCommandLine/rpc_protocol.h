#ifndef RPC_PROTOCOL_H
#define RPC_PROTOCOL_H
#define MAGIC 0x10203040
enum op_code
{
    ACKNOWLEDGE = 0,
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
    OPERATION = 0,
    CONTROl = 1,
    GET_PARAMETERS = 2,
    SET_PARAMETERS = 3,
    GET_STTAUS = 4,
    REPLY_MESSAGE = 6,
};
enum data0_acknowledge{
    ACK = 0,
    NACK = 1
};
enum data1_acknowledge
{
    COMMAND_ERROR,
    INVALID_MASSAGE,
    FAILED
};
enum data0_record{
    STOP,
    START,
    FORCE_STATR
};
enum snapshot_data{
    BMP,
    JPEG
};
enum error_bits{
STORAGE_ERROR,
DISK_FULL
};
enum control_data{
REBOOT,
SHUT_DOWN,
RESET_SETTINGS
};
enum status_bits{
    RECORD_BIT,
    SNAPSHOP_BIT
};
typedef struct{
    unsigned int magic;
    char Header_size;
    enum type type;
    enum op_code opcode;
    int data_size;
    int checksum;
    int reserve;
    char* Data;
}command_massage;
#endif // RPC_PROTOCOL_H
