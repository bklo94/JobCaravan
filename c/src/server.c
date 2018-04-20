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
#include <ctype.h>

struct InInput{
   char *jobtitle;
   char *city;
   char *state;
   char *response;
};

struct InInput fillIndeed(int, struct InInput);
char *replaceSpaces(char*);
void replaceNull(char*);

//devug with valgrind --leak-check=full -v /bin/server
//https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char *argv[]){
   char *request;
   int start = 0, end = 25, size;
   cJSON *response;
   struct InInput Indeed;
   do {
      Indeed = fillIndeed(start, Indeed);
      response = getRequest(Indeed.response);
      size = returnIndeedSize(response);
      returnIndeed(response);
      end = size;
      start+= 25;
      printf("%i, %i\n", start, end);
   } while(start <= end);
   return 0;
}

//TODO Create a loop to loop through all indeed results
struct InInput fillIndeed(int start, struct InInput temp){
   int radius = 90;
   char str2[1024];
   char *jobtitle = malloc(256);
   char *city = malloc(256);
   char *state = malloc(256);

   if (jobtitle == NULL || state == NULL || city == NULL){
      fprintf(stderr, "ERROR: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }

   if (start == 0){
      printf("Enter the jobtitle: ");
      fgets(jobtitle, 256, stdin);
      replaceNull(jobtitle);
      temp.jobtitle = replaceSpaces(jobtitle);

      printf("Enter the city: ");
      fgets(city, 256, stdin);
      replaceNull(city);
      temp.city = replaceSpaces(city);

      printf("Enter the State: ");
      fgets(state, 256, stdin);
      replaceNull(state);
      temp.state = replaceSpaces(state);
   }

   jobtitle = temp.jobtitle;
   city = temp.city;
   state = temp.state;
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
   temp.response = request;

   return temp;
}

char *replaceSpaces(char* string){
   size_t len = strlen(string);
   char *temp = malloc(len+1);
   strncpy(temp,string, len);
   for(int i = 0; i < strlen(temp); i++){
      if(isspace(temp[i]))
         temp[i] = '+';
   }
   string = temp;
   return string;
}

void replaceNull(char *string){
   if ((strlen(string) > 0) && (string[strlen (string) - 1] == '\n'))
      string[strlen (string) - 1] = '\0';
}
