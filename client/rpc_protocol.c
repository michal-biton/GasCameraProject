#include "rpc_protocol.h"

// Client side implementation of UDP client-server model
struct sockaddr_in servaddr;
command_massage* response;
int sockfd;

int init_socket()
{
    //init response
    response = (command_massage*)calloc(1, sizeof(command_massage));
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket creation failed");
        exit(ERROR);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    return SUCCESS;
}

int communicate_with_server(char* communication_buffer)
{
    char buffer[MAXLINE]={0};
    // Creating socket file descriptor
    init_socket();
    int n, len;
    n=sendto(sockfd, (const char *)communication_buffer, sizeof(command_massage),
             MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    printf("Hello message sent n=%d.\n",n);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
    memcpy(response, buffer, sizeof(command_massage));
    printf("Server : n=%dn", n);
    close(sockfd);
    if(communication_buffer)
        free(communication_buffer);
    return SUCCESS;
}

int create_request(char* data,u_int8_t type,u_int16_t opcode,int data_size)
{
    command_massage* request = (command_massage*)calloc(1, sizeof(command_massage));
    assert(request);
    char* buffer = (char*)calloc(sizeof(command_massage),sizeof(char));
    assert(buffer);
    request->magic = MAGIC;
    request->header_size = HEADER_SIZE;
    request->type = type;
    request->op_code = opcode;
    request->data_size = data_size;
    memcpy(request->data, data, data_size);
    request->checksum = request->header_size;
    for(int i=0; i<request->data_size; i++)
        request->checksum += data[i];
    memcpy(buffer, (void*)request, sizeof(command_massage));
    communicate_with_server(buffer);
    if(request)free(request);
    return SUCCESS;
}
