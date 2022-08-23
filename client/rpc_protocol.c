#include "rpc_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 8080
#define MAXLINE 1024

int communicate_with_server(char* buffer_communicate)
{
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    int n, len;
    sendto(sockfd, (const char *)buffer_communicate, strlen(buffer_communicate),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    printf("buffer_communicate message sent.\n");
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
    close(sockfd);
    if(buffer_communicate)
        free(buffer_communicate);
    return 0;
}

int request(char* data,enum type type,enum op_code op_code,int data_size)
{
    command_massage request;
    request.magic=MAGIC;
    request.header_size=HEADER_SIZE;
    request.type=type;
    request.opcode=op_code;
    request.data_size=data_size;
    request.checksum = 0;
    for(int i=0; i<request.data_size; i++)
        request.checksum += data[i];
    request.data=data;
    char* buffer=calloc(HEADER_SIZE+data_size,sizeof(char));
    strcat(buffer,(char*)(request.magic));
    strcat(buffer,(char*)request.header_size);
    strcat(buffer,(char*)request.type);
    strcat(buffer,(char*)request.opcode);
    strcat(buffer,(char*)request.data_size);
    strcat(buffer,(char*)request.checksum);
    strcat(buffer,(char*)request.data);
    communicate_with_server(buffer);
    return 1;
}


