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

int main(int argc, char const *argv[]) {
   int sock = 0, valread;
   struct sockaddr_in serv_addr;
   const char *message = "Hello from client.";
   char buffer[1024] = {0};

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      printf("Error Creating Socket.\n");
      return -1;
   }

   memset(&serv_addr, '0', sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(PORT);

   if (inet_pton(AF_INET, "10.0.0.85", &serv_addr.sin_addr) < 0){
      printf("Inavlid Address/Address not supported.\n");
      return -1;
   }

   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
      printf("Connection Failed.\n");
      return -1;
   }

   send(sock, message, strlen(message), 0);
   printf("Message Sent.\n");

   if ((valread = read(sock, buffer, 1024)) < 0 ){
      printf("Reading Message Failed.\n");
      return -1;
   }

   printf("%s\n", buffer);
   return 0;
}
