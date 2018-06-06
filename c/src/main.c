/**
* \author Brandon Lo
* \date 2018
* \copyright GNU GPL v3.0
*/

#include "API.h"
#include "requests.h"
#include "cJSON.h"
#include "server.h"
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
#include <omp.h>
#include <time.h>

#define NUMT 9

struct input{
   char *jobtitle;
   char *city;
   char *state;
   char *response;
};

struct state{
   char *name;
   int size;
   char *cities[10];
};

struct input fillZipRecruiter(int, char*, char*, char*, struct input);
struct input fillAuthentic(char*, struct input);
struct input fillAdzuna(int, char*, char*, char*, struct input);
struct input fillIndeed(int, char*, char*, char*,struct input);
char *replaceSpaces(char*);
void replaceNull(char*);
void *connection_handler(void*);
void callAPI();
void outputLog(char*);
void appendBuidlTime();

//devug with valgrind --leak-check=full -v /bin/server
//https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
//https://www.geeksforgeeks.org/socket-programming-cc/
//https://nominatim.openstreetmap.org/search?q=san%20francisco&format=json&polygon=1&addressdetails=1
//https://www.geeksforgeeks.org/socket-programming-python/

int main(int argc, char *argv[]){
   int runAPI = 0;
   #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
   #endif

   omp_set_num_threads(9);
   omp_set_nested(1);

   #pragma omp parallel num_threads(2)
   {
      while(1){
         if (omp_get_thread_num() == 0)
         {
               omp_set_num_threads(1);
               runAPI = listenCommand();
         }

         if (runAPI == 1)
         {
            appendBuidlTime();
            omp_set_num_threads(8);
            callAPI();
            runAPI = 0;
         }
      }
   }

   return 0;
}

void callAPI(){
   char *request;
   int start = 1, end = 25, size;
   cJSON *response;
   struct input Indeed, Adzuna, AuthenticJobs, ZipRecruiter;
   char jobArr[22][256] = {"Software+Engineer","Software+Developer","SDET","Junior+Engineer","Junior+Software","New+Grad+Engineer","New+Grad+Software","Software+Engineer+Apprentice","Computer+Science","Python+Developer","C+Developer","Java+Developer","Android+Developer","iOS+Developer","Javascript+Developer","Full+Stack","Front+End+Engineer","Back+End+Engineer","QA+Engineer","Quality+Assurance+Engineer","Devops","Forward+Deployed+Engineer"};
   static struct state CA, WA, NY, MA, IL, QC, CO, UT, GA, NC ,TX ,PA ,ON ,DC ,MI ,WI ,OR ,BC ,OH ,AB ,FL ,MN ,MO ,NV ,AZ ,NS ,AL, OK;
   CA.name = "CA";
   CA.size = 7;
   memcpy(CA.cities,(char *[7]){"San+Francisco","Oakland","San+Jose","Sacramento","Los+Angeles","Irvine","San+Diego"},7*sizeof(char*));
   WA.name = "WA";
   WA.size = 1;
   memcpy(WA.cities,(char *[1]){"Seattle"},1*sizeof(char*));
   NY.name = "NY";
   NY.size = 1;
   memcpy(NY.cities,(char *[1]){"New+York+City"},1*sizeof(char*));
   MA.name = "MA";
   MA.size = 1;
   memcpy(MA.cities,(char *[1]){"Boston"},1*sizeof(char*));
   IL.name = "IL";
   IL.size = 1;
   memcpy(IL.cities,(char *[1]){"Chicago"},1*sizeof(char*));
   QC.name = "QC";
   QC.size = 1;
   memcpy(QC.cities,(char *[1]){"Montreal"},1*sizeof(char*));
   CO.name = "CO";
   CO.size = 2;
   memcpy(CO.cities,(char *[2]){"Boulder", "Denver"},2*sizeof(char*));
   UT.name = "UT";
   UT.size = 2;
   memcpy(UT.cities,(char *[2]){"Salt+Lake+City","Provo"},2*sizeof(char*));
   GA.name = "GA";
   GA.size = 1;
   memcpy(GA.cities,(char *[1]){"Atlanta"},1*sizeof(char*));
   NC.name = "NC";
   NC.size = 3;
   memcpy(NC.cities,(char *[3]){"Charlotte", "Raleigh", "RTP"},3*sizeof(char*));
   TX.name = "TX";
   TX.size = 5;
   memcpy(TX.cities,(char *[5]){"Austin", "Dallas","Fort+Worth","Houston","San+Antonio"},5*sizeof(char*));
   PA.name = "PA";
   PA.size = 2;
   memcpy(PA.cities,(char *[2]){"Pittsburgh","Philadelphia"},2*sizeof(char*));
   ON.name = "ON";
   ON.size = 4;
   memcpy(ON.cities,(char *[4]){"Kitchener","Waterloo","Toronto","Ottawa"},4*sizeof(char*));
   DC.name = "DC";
   DC.size = 1;
   memcpy(DC.cities,(char *[1]){"Washington"},1*sizeof(char*));
   MI.name = "MI";
   MI.size = 3;
   memcpy(MI.cities,(char *[3]){"Ann+Arbor","Ypsilanti","Detroit"},3*sizeof(char*));
   WI.name = "WI";
   WI.size = 1;
   memcpy(WI.cities,(char *[1]){"Madison"},1*sizeof(char*));
   OR.name = "OR";
   OR.size = 1;
   memcpy(OR.cities,(char *[1]){"Portland"},1*sizeof(char*));
   BC.name = "BC";
   BC.size = 1;
   memcpy(BC.cities,(char *[1]){"Vancouver"},1*sizeof(char*));
   OH.name = "OH";
   OH.size = 2;
   memcpy(OH.cities,(char *[2]){"Columbus","Cleveland"},2*sizeof(char*));
   AB.name = "AB";
   AB.size = 2;
   memcpy(AB.cities,(char *[2]){"Edmonton","Calgary"},2*sizeof(char*));
   FL.name = "FL";
   FL.size = 3;
   memcpy(FL.cities,(char *[3]){"Tampa", "St.+Petersburg","Miami"},3*sizeof(char*));
   MN.name = "MN";
   MN.size = 2;
   memcpy(MN.cities,(char *[2]){"Minneapolis-Saint Paul", "Twin+Cities"},2*sizeof(char*));
   MO.name = "MO";
   MO.size = 3;
   memcpy(MO.cities,(char *[3]){"Kansas+City-Overland+Park", "St.Louis","Kansas+City"},3*sizeof(char*));
   NV.name = "NV";
   NV.size = 1;
   memcpy(NV.cities,(char *[1]){"Las+Vegas"},1*sizeof(char*));
   AZ.name = "AZ";
   AZ.size = 1;
   memcpy(AZ.cities,(char *[1]){"Phoenix"},1*sizeof(char*));
   NS.name = "NS";
   NS.size = 1;
   memcpy(NS.cities,(char *[1]){"Halifax"},1*sizeof(char*));
   AL.name = "AL";
   AL.size = 1;
   memcpy(AL.cities,(char *[1]){"Huntsville"},1*sizeof(char*));
   OK.name = "OK";
   OK.size = 1;
   memcpy(OK.cities,(char *[1]){"Oklahoma+City"},1*sizeof(char*));
   struct state arr[] = {CA, WA, NY, MA, IL, QC, CO, UT, GA, NC ,TX ,PA ,ON ,DC ,MI ,WI ,OR ,BC ,OH ,AB ,FL ,MN ,MO ,NV ,AZ ,NS ,AL, OK};

   for (int a = 0; a < sizeof(arr)/sizeof(arr[0]); a++){
      for (int b = 0; b < arr[a].size;b++){
         #pragma omp parallel for schedule(static) private(start, Indeed, response, size, end)
         for (int c = 0; c < sizeof(jobArr)/sizeof(jobArr[0]);c++){
            start = 0;
            do {
               Indeed = fillIndeed(start, jobArr[c], arr[a].cities[b], arr[a].name, Indeed);
               response = getRequest(Indeed.response);
               size = returnIndeedSize(response);
               returnIndeed(response);
               end = size;
               start+= 25;
               printf("%i, %i\n", start, end);
            } while(start <= end);
         }
      }
   }

   for (int a = 0; a < sizeof(arr)/sizeof(arr[0]); a++){
      for (int b = 0; b < arr[a].size;b++){
         #pragma omp parallel for schedule(static) private(start, ZipRecruiter, response, size, end)
         for (int c = 0; c < sizeof(jobArr)/sizeof(jobArr[0]);c++){
            start = 0;
            do {
               ZipRecruiter = fillZipRecruiter(start,jobArr[c],arr[a].cities[b],arr[a].name,ZipRecruiter);
               response = getRequest(ZipRecruiter.response);
               size = returnRecruiterSize(response);
               size /= 100;
               returnZipRecruiter(response);
               end = size;
               start++;
               printf("%i, %i\n", start, end);
            } while(start <= end);
         }
      }
   }

   for (int a = 0; a < sizeof(arr)/sizeof(arr[0]); a++){
      for (int b = 0; b < arr[a].size;b++){
         #pragma omp parallel for schedule(static) private(AuthenticJobs, response)
         for (int c = 0; c < sizeof(jobArr)/sizeof(jobArr[0]);c++){
            //Low rate limit, may delete from application
            //Authentic has almost no Software dev jobs... So no need to loop
            AuthenticJobs = fillAuthentic(jobArr[c],AuthenticJobs);
            response = getRequest(AuthenticJobs.response);
            returnAuthentic(response);
         }
      }
   }

   for (int a = 0; a < sizeof(arr)/sizeof(arr[0]); a++){
      for (int b = 0; b < arr[a].size;b++){
         #pragma omp parallel for schedule(static) private(start, Adzuna, response, size, end)
         for (int c = 0; c < sizeof(jobArr)/sizeof(jobArr[0]);c++){
            start = 1;
            do {
               Adzuna = fillAdzuna(start, jobArr[c], arr[a].cities[b], arr[a].name, Adzuna);
               response = getRequest(Adzuna.response);
               size = returnAdzunaSize(response);
               size /= 50;
               returnAdzuna(response);
               end = size;
               start++;
               printf("%i, %i\n", start, end);
            } while(start <= end);
         }
      }
   }

   return;
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
   outputLog(request);
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
   outputLog(request);
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
   outputLog(request);
   temp.response = request;

   return temp;
}

struct input fillZipRecruiter(int start, char* jobtitle, char* city, char *state, struct input temp){
   //larger than length of continential United States
   int radius = 200;
   char str1[1024];
   temp.jobtitle = jobtitle;
   temp.city = city;
   temp.state = state;
   int num = snprintf(str1, 1024, "https://api.ziprecruiter.com/jobs/v1?search=%s&location=%s+%s&radius_miles=%i&days_ago=&jobs_per_page=500&page=%i&api_key=",jobtitle, city, state, radius, start);
   if (num >sizeof(str1)){
      fprintf(stderr, "ERROR server.c: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }

   char *request = concatEndAPI(str1,ZIP_KEY);
   outputLog(request);
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

void outputLog(char *buffer){
   FILE *outputLog = fopen("output.log", "ab+");
   if (outputLog == NULL){
      printf("Error opening file!\n");
      exit(1);
   }
   fprintf(outputLog,"%s\n\n",buffer);
   fclose(outputLog);
   return;
}

void appendBuidlTime(){
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   FILE *outputLog = fopen("output.log", "ab+");
   if (outputLog == NULL){
      printf("Error opening file!\n");
      exit(1);
   }
   fprintf(outputLog,"Build Time: %s\n\n",asctime(tm));
   fclose(outputLog);
}
