#include "API.h"
#include "requests.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<fcntl.h>
#include <sys/types.h>


//https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char *argv[]){
    char *request = "/films/58611129-2dbc-4a81-a72f-77ddfc1b1b49";
    char *html = "ghibliapi.herokuapp.com";
    getRequest(request,html);

    return 0;
}
