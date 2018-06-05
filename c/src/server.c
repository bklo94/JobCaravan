/**
* \author Brandon Lo
* \date 2018
* \copyright GNU GPL v3.0
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 10800

int listenCommand(){
   int server_fd, new_socket, valread;
   struct sockaddr_in address;
   int opt = 1;
   int addrlen = sizeof(address);
   char buffer[1024] = {0};
   char *response = "Hello from the server!";

   server_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (server_fd == 0){
      perror("Could not create socket.");
      exit(EXIT_FAILURE);
   }
   if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
      perror("Attaching Socket Failure.");
      exit(EXIT_FAILURE);
   }

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);

   if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
      perror("Binding Failed.");
      exit(EXIT_FAILURE);
   }

   if (listen(server_fd, 3) < 0){
      perror("Listening Failed.");
      exit(EXIT_FAILURE);
   }

   if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
      perror("Accepting Socket Failed");
      exit(EXIT_FAILURE);
   }

   if ((valread = read(new_socket, buffer, 1024)) < 0 ){
      perror("Reading Message Failed.");
      exit(EXIT_FAILURE);
   }

   printf("%s\n", buffer);
   send(new_socket, response, strlen(response), 0);
   printf("Hello message sent!\n");

   return 1;
}
