#include "rpc_protocol_server.h"

extern p_handler handler;
command_massage* ack_response;
struct sockaddr_in servaddr, cliaddr;
int sockfd;

int init_socket()
{
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    return EXIT_SUCCESS;
}

int communicate_with_client(char* send_message)
{
    char buffer[MAXLINE]={0};
    while(1)
    {
        if ( bind(sockfd, (const struct sockaddr *)&servaddr,
                  sizeof(servaddr)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        int len, n;
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        //buffer[n] = '\0';
        recieve_request((char*)buffer);
       n = sendto(sockfd, (const char *)send_message, sizeof (command_massage),
               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
        printf("n=%d Hello message sent.\n",n);
    }
    return 0;
}

int read_massege(char* buffer,command_massage *request)
{
    memcpy(request,(void*)buffer,sizeof(command_massage));
    char magic[4];
    sprintf(magic,"%ud",MAGIC);
    while(*buffer && strncmp(buffer,magic,4))
        buffer++;
    if(!*buffer)
    {
        //NACK INVALID_MEASSAGE//@ How to return the message if it does not exist???
        return EXIT_FAILURE;
    }
    request->magic=MAGIC;
    char data_size[5]={'\0'};
    memcpy(data_size,buffer,4);
    return EXIT_SUCCESS;
}

int init_response()
{
    ack_response = NULL;
    ack_response = (command_massage*)calloc(1,sizeof(command_massage));
    assert(acknowledge);
    ack_response->magic = MAGIC;
    ack_response->header_size = HEADER_SIZE;
    ack_response->type = ACKNOWLEDGE_TYPE;
    ack_response->opcode = ACKNOWLEDGE;
    ack_response->data_size = 3;
    ack_response->checksum = 0;
    ack_response->data[0] = '\0';
    return EXIT_SUCCESS;
}

int free_acknowledge()
{
    if(ack_response)
    {
        free(ack_response);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

char* acknowledge(uint8_t ack,uint8_t nack_type,uint8_t num_type)//@ if buffer not NULL
{
    if(ack)
        ack_response->data[0] = ACK;
    else
    {
        ack_response->data[0] = NACK;
        ack_response->data[1] = nack_type;
        if(nack_type == FAILED)
            ack_response->data[2] = num_type;
    }
    char* res = (char*)calloc(sizeof (command_massage),sizeof (char));
    memcpy(res,(void*)ack_response, sizeof (command_massage));
    communicate_with_client(res);
    return EXIT_SUCCESS;
}

int act_command(command_massage *request)
{
    switch(request->opcode)
    {
    case RECORD:{
        if(request->data[0] == STOP)
            GAS_API_STOP_RECORD_CLI(0, &request->data);
        else if(request->data[0] == START)
        {
            GAS_API_START_RECORD_CLI(2, &request->data);
            if(request->data[1]){
                char* time=request->data+1;
                sleep(atoi(time));
                GAS_API_STOP_RECORD_CLI();
            }
        }
        else if(request->data[0] == FORCE_STATR)
        {
            GAS_API_STOP_RECORD_CLI();
            GAS_API_START_RECORD_CLI();
        }
        else
            acknowledge(NACK, COMMAND_ERROR,0);
        break;
    }
    case SNAPSHOT:
    {
        GAS_API_DO_SNAPSHOT_CLI();
        break;
    }
    case GET_RECORD_PARAMS:
        //GET_RECORD_PARAMS_CLI();
        break;
    case SET_RECORD_PARAMS:
        //SET_RECORD_PARAMS_CLI(0,&request->data);
        break;
    case GET_SNAPSHOT_PARAMS:
        //GET_SNAPSHOT_PARAMS_CLI();
        break;
    case SET_SNAPSHOT_PARAMS:
        //SET_SNAPSHOT_PARAMS_CLI(0,&request->data);
        break;
    case GET_STATUS:
        GAS_API_GET_STATUS_CLI();
        break;
    }
    return EXIT_SUCCESS;
}

int recieve_request(char* buffer)
{
    command_massage *request=(command_massage*)malloc(sizeof(command_massage));
    if(read_massege(buffer,request))
        return EXIT_FAILURE;
    act_command(request);
    return EXIT_SUCCESS;

}


