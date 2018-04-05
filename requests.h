#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<fcntl.h>
#include <sys/types.h>

void error(const char *msg) { perror(msg); exit(0); }

void getRequest(char *requests, char *URL){

    int portNum = 80;
    char *host = URL;
    char *messageFormat = "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total, rv;
    char message[1024], response[4096];

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec= 0;

    sprintf(message, messageFormat, requests,URL);
    printf("\n%s\n", message);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNum);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    total = strlen(message);
    sent = 0;
    do{
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent += bytes;
    }while (sent < total);

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&timeout, sizeof(struct timeval)) < 0 ) error("ERROR Timeout socket setup error");
    printf("Getting Message\n\n");
    memset(response,0,sizeof(response));
    total = sizeof(response - 1);
    received = 0;
    do{
        printf("%s", response);
        memset(response, 0, sizeof(response));
        bytes = recv(sockfd, response, 1024, 0);
        if (bytes < 0)
            printf("ERROR reading response from socket");
        if (bytes <= 0)
            break;
        received += bytes;
    } while(1);

    close(sockfd);
}

char* concatAPI(char *a, const char *b, char *c){
    size_t len = strlen(a) + strlen(b) + strlen(c);
    char *str = malloc(len + 1);
    strcpy(str,a);
    strcat(str,b);
    strcat(str,c);
    return str;
}

#endif
