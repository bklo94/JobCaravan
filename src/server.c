#include "API.h"
#include "requests.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <curl/curl.h>

char *itoa(int num, char *str);

//https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char *argv[]){
   int start = 0, end = 25, radius = 90;
   char str2[1024];
   cJSON *response, *size, *string;
   char *jobtitle = "python+developer";
   char *city = "san+francisco";
   char *state = "CA";
   char *jobType = "fulltime";
   char *Firefox1 = "%2F";
   char *Firefox2 = "%28F";
   char *str1 = "api.indeed.com/ads/apisearch?publisher=";
   int num = snprintf(str2, 1024, "&q=%s&l=%s+%s&sort=&radius=%i&st=&jt=%s&start=%i&limit=25&fromage=&filter=&latlong=1&co=us&chnl=&userip=&useragent=Mozilla/%s4.0%s(Firefox)&v=2&format=json",jobtitle, city, state, radius, jobType, start, Firefox1, Firefox2);
   if (num >sizeof(str2)){
      fprintf(stderr, "ERROR: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }
   char *request = concatAPI(str1,INDEED_KEY,str2);
   response = getRequest(request);
   size = returnIndeedSize(response);
   returnIndeed(response);
   return 0;
}

void fillIndeed(){

}
