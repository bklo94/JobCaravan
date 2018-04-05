#include "API.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) { perror(msg); exit(0); }

//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char *argv[]){
    int portNum = 80;
    char *host = "ghibliapi.herokuapp.com";
    char *messageFormat = "GET /films/58611129-2dbc-4a81-a72f-77ddfc1b1b49 HTTP/1.1\r\nHost: ghibliapi.herokuapp.com\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024], response[128000];

    //if (argc < 3) {puts("Parameters: <apikey> <command>"); exit(0);}

    sprintf(message, messageFormat);
    printf("\n%s\n", message);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd <0) error("ERROR opening socket");

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

    memset(response,0,sizeof(response));
    total = sizeof(response - 1);
    received = 0;
    do{
        printf("RESPONSE: %s\n", response);
        memset(response, 0, sizeof(response));
        bytes = recv(sockfd, response, 1023, 0);
        if (bytes < 0)
            printf("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received += bytes;
    } while(1);

    close(sockfd);
    exit(0);
    return 0;
}
