#include "rpc_protocol.h"
// Server side implementation of UDP client-server model

extern p_handler handler;
command_massage* response;
struct sockaddr_in servaddr, cliaddr;
int sockfd;

int init_socket()
{
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int communicate_with_client()
{
    char buffer[MAXLINE] = {0};
    char send_message[MAXLINE] = {0};
    response = (command_massage*)calloc(1, sizeof(command_massage));
    assert(response);
    while(WORKFOREVER)
    {
        // Bind the socket with the server address
        int len, n;
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr*) &cliaddr, &len);
        //printf("%s.%d n=%d\n\r",__func__,__LINE__,n);
        recieve_request((char*)buffer);
        memcpy(send_message, (void*)response, sizeof(command_massage));
        n = sendto(sockfd, (const char*)send_message, sizeof(command_massage),
                   MSG_CONFIRM, (const struct sockaddr*) &cliaddr,len);
        printf("n=%d Hello message sent.\n",n);
    }
    return SUCCESS;
}

int read_massege(char* buffer,command_massage *request){
    int checksum = 0;
    memcpy(request,(void*)buffer,sizeof(command_massage));
    /*
    while(*buffer && strncmp(buffer,magic,4))
        buffer++;
    */
    checksum = response->header_size;
    for(int i=0; i<response->data_size; i++){
        response->checksum += response->data[i];
    }
    if(checksum != request->checksum)
        acknowledge(NACK, INVALID_MASSAGE, 0);
    return SUCCESS;
}

int free_respognse(){
    if(response){
        free(response);
        return SUCCESS;
    }
    return ERROR;
}

int acknowledge(uint8_t ack, uint8_t nack_type, uint8_t num_type)
{
    char data[3]={0};
    if(ack)
        data[0] = ACK;
    else {
        data[0] = NACK;
        data[1] = nack_type;
        if(nack_type == FAILED)
        {
            data[2] = num_type;
        }
    }
    create_response(ACKNOWLEDGE_TYPE, ACKNOWLEDGE, 3, data);
    return SUCCESS;
}

int create_response(uint8_t type,uint16_t opcode,int data_size,char data[MAXLINE]){
    response->magic = MAGIC;
    response->header_size = HEADER_SIZE;//size 20 for all with 7 reserve
    response->type = type;
    response->opcode = opcode;
    response->data_size = data_size;
    memcpy(response->data, data, data_size);
    response->checksum = response->header_size;
    for(int i=0; i<response->data_size; i++){
        response->checksum += data[i];
    }
    return SUCCESS;
}

int act_command(command_massage *request){
    switch(request->opcode){
    case RECORD:{
        if(request->data[0] == STOP)
            GAS_API_STOP_RECORD_WRAPER();
        else if(request->data[0] == START){
            GAS_API_START_RECORD_WRAPER();
            if(request->data[1]){
                char* time = request->data+1;
                sleep(atoi(time));
                GAS_API_STOP_RECORD_WRAPER();
            }
        }
        else if(request->data[0] == FORCE_STATR){
            GAS_API_STOP_RECORD_WRAPER();
            GAS_API_START_RECORD_WRAPER();
        }
        else
            acknowledge(NACK, COMMAND_ERROR,0);
        break;
    }
    case SNAPSHOT:{
        GAS_API_DO_SNAPSHOT_WRAPER();
        break;
    }
    case GET_RECORD_PARAMS:
        GAS_API_GET_RECORD_PARAMS_WRAPER();
        break;
    case SET_RECORD_PARAMS:
        GAS_API_SET_RECORD_PARAMS_WRAPER(request->data);
        break;
    case GET_SNAPSHOT_PARAMS:
        GAS_API_GET_SNAPSHOT_PARAMS_WRAPER();
        break;
    case SET_SNAPSHOT_PARAMS:
        GAS_API_SET_SNAPSHOT_PARAMS_WRAPER(request->data);
        break;
    case GET_STATUS:
        GAS_API_GET_STATUS_WRAPER();
        break;
    }
    return SUCCESS;
}

int recieve_request(char* buffer){
    command_massage *request = (command_massage*)calloc(1,sizeof(command_massage));
    if(read_massege(buffer,request))
        return ERROR;
    act_command(request);
    return SUCCESS;
}
