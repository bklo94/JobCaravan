/**
* \author Brandon Lo
* \date 2018
* \copyright GNU GPL v3.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 10800

int listenCommand(){
   int socket_desc;
   struct sockaddr_in server;

   socket_desc = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_desc == -1){
      printf("Could not create socket.\n");
      exit(1);
   }
   server.sin_addr.s_addr = inet_addr("10.0.0.1");
   server.sin_family = AF_INET;
   server.sin_port = htons(80);
   if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
      printf("Connection Error\n");
      exit(1);
   }

   printf("Connected\n");
   return 1;
}
