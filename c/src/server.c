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

struct input{
   char *jobtitle;
   char *city;
   char *state;
   char *response;
};

struct input fillGithub(char*, struct input);
struct input fillAuthentic(char*, struct input);
struct input fillAdzuna(int, char*, char*, char *, struct input);
struct input fillIndeed(int, char*, char*, char *,struct input);
char *replaceSpaces(char*);
void replaceNull(char*);

//devug with valgrind --leak-check=full -v /bin/server
//https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char *argv[]){
   char *request;
   int start = 1, end = 25, size;
   cJSON *response;
   struct input Indeed, Adzuna, AuthenticJobs, ZipRecruiter;
   char *jobtitle = malloc(256);
   char *city = malloc(256);
   char *state = malloc(256);
   int testKey = 1;

   if(testKey == 0){
      if (jobtitle == NULL || state == NULL || city == NULL){
         fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
         exit(1);
      }
      printf("Enter the jobtitle: ");
      fgets(jobtitle, 256, stdin);
      replaceNull(jobtitle);
      jobtitle = replaceSpaces(jobtitle);

      printf("Enter the city: ");
      fgets(city, 256, stdin);
      replaceNull(city);
      city = replaceSpaces(city);

      printf("Enter the State: ");
      fgets(state, 256, stdin);
      replaceNull(state);
      state = replaceSpaces(state);
   }

   else{
      jobtitle = "Software+Developer";
      city = "San+Francisco";
      state = "CA";
   }

   //Authentic has almost no Software dev jobs... So no need to loop
   AuthenticJobs = fillAuthentic(jobtitle,AuthenticJobs);
   response = getRequest(AuthenticJobs.response);
   returnAuthentic(response);

   do {
      Adzuna = fillAdzuna(start, jobtitle, city, state, Adzuna);
      response = getRequest(Adzuna.response);
      size = returnAdzunaSize(response);
      size /= 50;
      returnAdzuna(response);
      end = size;
      start++;
      printf("%i, %i\n", start, end);
   } while(start <= end);

   do {
      Indeed = fillIndeed(start, jobtitle, city, state, Indeed);
      response = getRequest(Indeed.response);
      size = returnIndeedSize(response);
      returnIndeed(response);
      end = size;
      start+= 25;
      printf("%i, %i\n", start, end);
   } while(start <= end);

   return 0;
}

struct input fillAdzuna(int start, char* jobtitle, char* city, char *state,struct input temp){
   char str1[1024];
   char str3[1024];
   temp.jobtitle = jobtitle;
   temp.city = city;
   temp.state = state;
   int num = snprintf(str1, 1024, "api.adzuna.com/v1/api/jobs/us/search/%i?app_id=",start);
   if (num >sizeof(str1)){
      fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }
   char *str2 = "&app_key=";
   num = snprintf(str3, 1024, "&results_per_page=1000&what=%s&where=%s+%s&distance=200&full_time=1",jobtitle, city, state);
   if (num >sizeof(str3)){
      fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }
   char *request = concatAPI(str1,ADZUNA_APPID,str2);
   request = concatAPI(request,ADZUNA_KEY,str3);
   temp.response = request;

   return temp;
}

struct input fillIndeed(int start, char* jobtitle, char* city, char *state, struct input temp){
   int radius = 90;
   char str2[1024];
   temp.jobtitle = jobtitle;
   temp.city = city;
   temp.state = state;
   char *jobType = "fulltime";
   char *Firefox1 = "%2F";
   char *Firefox2 = "%28F";
   char *str1 = "api.indeed.com/ads/apisearch?publisher=";
   int num = snprintf(str2, 1024, "&q=%s&l=%s+%s&sort=&radius=%i&st=&jt=%s&start=%i&limit=25&fromage=&filter=&latlong=1&co=us&chnl=&userip=&useragent=Mozilla/%s4.0%s(Firefox)&v=2&format=json",jobtitle, city, state, radius, jobType, start, Firefox1, Firefox2);
   if (num >sizeof(str2)){
      fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }

   char *request = concatAPI(str1,INDEED_KEY,str2);
   temp.response = request;

   return temp;
}

struct input fillAuthentic(char* jobtitle, struct input temp){
   char *str1;
   char str2[1024];
   temp.jobtitle = jobtitle;
   str1 = "https://authenticjobs.com/api/?api_key=";
   int num = snprintf(str2, 1024, "&format=json&method=aj.jobs.search&keywords=%s&perpage=100",jobtitle);
   if (num >sizeof(str2)){
      fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }
   char *request = concatAPI(str1,AUTHENTIC_KEY,str2);
   temp.response = request;

   return temp;
}

struct input fillZipRecruiter(int start, char* jobtitle, char* city, char *state, struct input temp){
   //larger than length of continential United States
   int radius = 3000;
   char str1[1024];
   temp.jobtitle = jobtitle;
   temp.city = city;
   temp.state = state;
   int num = snprintf(str1, 1024, "api.ziprecruiter.com/jobs/v1?search=%s&location=%s+%s&radius_miles=%s&days_ago=&jobs_per_page=500&page=%s&api_key=",jobtitle, city, state, radius, start);
   if (num >sizeof(str1)){
      fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }

   char *request = concatEndAPI(str1,ZIP_KEY);
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
