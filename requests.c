#include "API.h"
#include "requests.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>

//https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char *argv[]){

    char *str1 = "https://api.indeed.com/ads/apisearch?publisher=";
    char *str2 = "&q=python+developer&l=san%20francisco%2C+ca&sort=&radius=90&st=&jt=fulltime&start=&limit=25&fromage=&filter=&latlong=1&co=us&chnl=&userip=1.2.3.4&useragent=Mozilla/%2F4.0%28Firefox%29&v=2&format=json";
    char *request = concatAPI(str1,INDEED_KEY,str2);

    getRequest(request);

    return 0;
}
