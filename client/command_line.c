#include "command_line.h"

extern command_massage* response;

int GAS_API_START_RECORD_CLI()
{
    char data[8] = {0};
    data[0] = START;
    data[1] = FOREVER;
    create_request((char*)data, OPERATION, RECORD, 8);
    return SUCCESS;
}

int GAS_API_STOP_RECORD_CLI()
{
    char data[2];
    data[0] = 0;
    data[1] = 0;
    create_request((char*)data, OPERATION, RECORD, 2);
    return SUCCESS;
}

int GAS_API_DO_SNAPSHOT_CLI()
{
    create_request("", OPERATION, SNAPSHOT, 0);
    return SUCCESS;
}

int GAS_API_GET_STATUS_CLI()
{
    create_request("", GET_STATUS, SNAPSHOT, 0);
    return SUCCESS;
}

int GAS_API_GET_RECORD_PARAMS()
{
    record_t* record = (record_t*)calloc(1, sizeof(record_t));
    assert(record);
    create_request("", GET_PARAMETERS, GET_RECORD_PARAMS, 0);
    memcpy(record, response->data, sizeof(record_t));
    printf("\n path:%s height:%d width:%d\n",record->file_name ,record->height, record->width);
    if(record)free(record);
    return SUCCESS;
}

int GAS_API_SET_RECORD_PARAMS(char** data)
{
    record_t* record = (record_t*)calloc(1, sizeof(record_t));
    assert(record);
    char* buffer = (char*)calloc(1,sizeof(record));
    assert(buffer);
    //strcpy(record->file_name, data[1]);
    record->file_name = strdup(data[1]);
    record->codec = atoi(data[2]);
    record->height = atoi(data[3]);
    record->width = atoi(data[4]);
    record->fps = atoi(data[5]);
    memcpy(buffer, record, sizeof (record_t));
    create_request(buffer,SET_PARAMETERS, SET_SNAPSHOT_PARAMS, sizeof(record_t) );
    if(record)free(record);
    return SUCCESS;
}

int GAS_API_GET_SNAPSHOT_PARAMS()
{
    snapshot_t* snapshot = (snapshot_t*)calloc(1, sizeof(snapshot_t));
    assert(snapshot);
    create_request("", GET_SNAPSHOT_PARAMS, SNAPSHOT, 0);
    memcpy(snapshot, response->data, sizeof(record_t));
    printf("filepath:%s height:%d width:%d ", snapshot->file_name, snapshot->height, snapshot->width);
    if(snapshot)free(snapshot);
    return SUCCESS;
}

int GAS_API_SET_SNAPSHOT_PARAMS(char** data)
{
    snapshot_t* snapshot = (snapshot_t*)calloc(1, sizeof(snapshot_t));
    if(!snapshot)exit(ERROR);
    char* buffer = (char*)calloc(1,sizeof(snapshot));
    if(!buffer)exit(ERROR);
    //strcpy(snapshot->file_name, data[1]);
    snapshot->file_name = strdup(data[1]);
    snapshot->type = atoi(data[2]);
    snapshot->height = atoi(data[3]);
    snapshot->width = atoi(data[4]);
    memcpy(buffer, snapshot, sizeof (snapshot_t));
    create_request(buffer,SET_PARAMETERS, SET_SNAPSHOT_PARAMS, sizeof(snapshot_t) );
    if(snapshot)free(snapshot);
    return SUCCESS;
}

