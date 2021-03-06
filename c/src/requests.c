/**
* \author Brandon Lo
* \date 2018
* \copyright GNU GPL v3.0
*/

#include "database.h"
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

//basic way to print out results and check if they are pulling correctly.
//printf("%s, %s, %s, %s, %s, %s, %s, %s\n", jobtitle->valuestring, company->valuestring, snippet->valuestring, url->valuestring, postDate->valuestring, relDate->valuestring, city->valuestring, state->valuestring);
struct MemoryStruct {
  char *memory;
  size_t size;
};

void error(const char *msg) {
   perror(msg);
   exit(0);
}

static size_t writeMemoryCallBack(void *contents, size_t size, size_t nmemb, void *userp){
   size_t realsize = size * nmemb;
   struct MemoryStruct *mem = (struct MemoryStruct*)userp;

   mem->memory = realloc(mem->memory, mem->size + realsize + 1);
   if(mem->memory == NULL){
      printf("ERROR: Not enough memory");
      return 0;
   }

   memcpy(&(mem->memory[mem->size]), contents, realsize);
   mem->size += realsize;
   mem->memory[mem->size] = 0;

   return realsize;
}

char* concatAPI(char *a, const char *b, char *c){
    size_t len = strlen(a) + strlen(b) + strlen(c);
    char *str = malloc(len + 1);
    strcpy(str,a);
    strcat(str,b);
    strcat(str,c);
    return str;
}

char* concatEndAPI(char *a, const char *b){
    size_t len = strlen(a) + strlen(b);
    char *str = malloc(len + 1);
    strcpy(str,a);
    strcat(str,b);
    return str;
}

char* concatThree(char *a, char *b, char *c){
    size_t len = strlen(a) + strlen(b) + strlen(c);
    char *str = malloc(len + 1);
    strcpy(str,a);
    strcat(str,b);
    strcat(str,c);
    return str;
}

//https://curl.haxx.se/libcurl/c/https.html
//https://curl.haxx.se/libcurl/c/getinmemory.html
cJSON* getRequest(char *URL){
   CURL *curl_handle;
   CURLcode res;
   struct MemoryStruct chunk;
   chunk.memory = malloc(1);
   chunk.size = 0;
   cJSON *response = 0;
   curl_global_init(CURL_GLOBAL_ALL);
   printf("%s\n", URL);
   curl_handle = curl_easy_init();
   curl_easy_setopt(curl_handle, CURLOPT_URL,URL);
   curl_easy_setopt(curl_handle,CURLOPT_WRITEFUNCTION,writeMemoryCallBack);
   curl_easy_setopt(curl_handle,CURLOPT_WRITEDATA,(void *)&chunk);
   curl_easy_setopt(curl_handle,CURLOPT_USERAGENT,"libcurl-agent/1.0");
   res = curl_easy_perform(curl_handle);
   if(res != CURLE_OK){
      fprintf(stderr, "ERROR: curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      curl_easy_cleanup(curl_handle);
      free(chunk.memory);
      curl_global_cleanup();
   }
   //How to iterate through a cJSON object
   //https://stackoverflow.com/questions/16900874/using-cjson-to-read-in-a-json-array/16901333
   else{
      response = cJSON_Parse(chunk.memory);
      char *string2 = cJSON_Print(response);
      if (!string2){
    		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
         curl_easy_cleanup(curl_handle);
         free(chunk.memory);
         curl_global_cleanup();
    	}
      free(string2);
   }

   curl_easy_cleanup(curl_handle);
   free(chunk.memory);
   curl_global_cleanup();

   return response;
}

void returnIndeed(cJSON *response){
   int i;
   cJSON *jobtitle, *company, *city, *state, *snippet, *latitude, *longitude, *url, *array, *relDate, *postDate;
   response = cJSON_GetObjectItem(response,"results");
   for (i =0; i < cJSON_GetArraySize(response); i++){
      array = cJSON_GetArrayItem(response,i);
      jobtitle = cJSON_GetObjectItem(array,"jobtitle");
      company = cJSON_GetObjectItem(array,"company");
      city = cJSON_GetObjectItem(array,"city");
      state = cJSON_GetObjectItem(array,"state");
      snippet = cJSON_GetObjectItem(array,"snippet");
      latitude = cJSON_GetObjectItem(array,"latitude");
      longitude = cJSON_GetObjectItem(array,"longitude");
      url = cJSON_GetObjectItem(array,"url");
      relDate = cJSON_GetObjectItem(array,"formattedRelativeTime");
      postDate = cJSON_GetObjectItem(array,"date");
      insertIndeedDB(jobtitle->valuestring, company->valuestring, city->valuestring, state->valuestring, snippet->valuestring, url->valuestring, longitude->valuedouble, latitude->valuedouble, relDate->valuestring, postDate->valuestring);
   }
}

void returnAdzuna(cJSON *response){
   int i;
   cJSON *jobtitle, *company, *city, *state, *snippet, *latitude, *longitude, *url, *array, *postDate;
   response = cJSON_GetObjectItem(response,"results");
   for (i =0; i < cJSON_GetArraySize(response); i++){
      array = cJSON_GetArrayItem(response,i);
      jobtitle = cJSON_GetObjectItem(array,"title");
      company = cJSON_GetObjectItem(array,"company");
      if (cJSON_GetObjectItem(company,"display_name") == NULL){
         company = cJSON_GetObjectItem(array,"title");
      }
      else{
         company = cJSON_GetObjectItem(company,"display_name");
      }
      snippet = cJSON_GetObjectItem(array,"description");
      latitude = cJSON_GetObjectItem(array,"latitude");
      longitude = cJSON_GetObjectItem(array,"longitude");
      url = cJSON_GetObjectItem(array,"redirect_url");
      postDate = cJSON_GetObjectItem(array,"created");
      city = cJSON_GetObjectItem(array,"location");
      city = cJSON_GetObjectItem(city,"area");
      if (cJSON_GetArraySize(city) == 4){
         city = cJSON_GetArrayItem(city,3);
         state = cJSON_GetObjectItem(array,"location");
         state = cJSON_GetObjectItem(state,"area");
         state = cJSON_GetArrayItem(state,1);
      }
      else if (cJSON_GetArraySize(city) == 3){
         city = cJSON_GetArrayItem(city,2);
         state = cJSON_GetObjectItem(array,"location");
         state = cJSON_GetObjectItem(state,"area");
         state = cJSON_GetArrayItem(state,1);
      }
      else if (cJSON_GetArraySize(city) == 2){
         city = cJSON_GetArrayItem(city,1);
         state = cJSON_GetObjectItem(array,"location");
         state = cJSON_GetObjectItem(state,"area");
         state = cJSON_GetArrayItem(state,1);
      }
      else{
         city = cJSON_GetArrayItem(city,0);
         state = cJSON_GetObjectItem(array,"location");
         state = cJSON_GetObjectItem(state,"area");
         state = cJSON_GetArrayItem(state,1);
      }
      insertAdzunaDB(jobtitle->valuestring, company->valuestring, city->valuestring, state->valuestring, snippet->valuestring, url->valuestring, longitude->valuedouble, latitude->valuedouble, postDate->valuestring);
   }
}

void returnAuthentic(cJSON *response){
   int i;
   double latitude, longitude;
   cJSON *jobtitle, *company, *city, *state, *snippet, *url, *array, *postDate;
   response = cJSON_GetObjectItem(response,"listings");
   response = cJSON_GetObjectItem(response,"listing");
   for (i =0; i < cJSON_GetArraySize(response); i++){
      array = cJSON_GetArrayItem(response,i);
      jobtitle = cJSON_GetObjectItem(array,"title");
      company = cJSON_GetObjectItem(array,"company");
      company = cJSON_GetObjectItem(company,"name");
      snippet = cJSON_GetObjectItem(array,"keywords");
      latitude = 0.0;
      longitude = 0.0;
      url = cJSON_GetObjectItem(array,"apply_url");
      postDate = cJSON_GetObjectItem(array,"post_date");
      city = cJSON_GetObjectItem(array,"company");
      if (cJSON_GetObjectItem(city,"location") == NULL){
         city = company;
         state = company;
      }
      else{
         city = cJSON_GetObjectItem(city,"location");
         if (cJSON_GetObjectItem(city,"city") == NULL){
            city = cJSON_GetObjectItem(city,"city");
            state = cJSON_GetObjectItem(array,"company");
            state = cJSON_GetObjectItem(state,"location");
            state = cJSON_GetObjectItem(state,"state");
         }
         else{
            city = company;
            state = company;
         }
      }
      insertAuthenticDB(jobtitle->valuestring, company->valuestring, city->valuestring, state->valuestring, snippet->valuestring, url->valuestring, longitude, latitude, postDate->valuestring);
   }
}

void returnZipRecruiter(cJSON *response){
   int i;
   double latitude, longitude;
   cJSON *jobtitle, *company, *city, *state, *snippet, *url, *array, *relDate, *postDate;
   response = cJSON_GetObjectItem(response,"jobs");
   for (i =0; i < cJSON_GetArraySize(response); i++){
      array = cJSON_GetArrayItem(response,i);
      jobtitle = cJSON_GetObjectItem(array,"name");
      company = cJSON_GetObjectItem(array,"hiring_company");
      company = cJSON_GetObjectItem(company,"name");
      snippet = cJSON_GetObjectItem(array,"snippet");
      latitude = 0.0;
      longitude = 0.0;
      url = cJSON_GetObjectItem(array,"url");
      postDate = cJSON_GetObjectItem(array,"posted_time");
      relDate = cJSON_GetObjectItem(array,"posted_time_friendly");
      city = cJSON_GetObjectItem(array,"city");
      state = cJSON_GetObjectItem(array,"state");
      insertZipDB(jobtitle->valuestring, company->valuestring, city->valuestring, state->valuestring, snippet->valuestring, url->valuestring, longitude, latitude, relDate->valuestring, postDate->valuestring);
   }
}

int returnIndeedSize(cJSON *response){
   cJSON *size = cJSON_GetObjectItem(response,"totalResults");
   return size->valueint;
}

int returnAdzunaSize(cJSON *response){
   cJSON *size = cJSON_GetObjectItem(response,"count");
   return size->valueint;
}

int returnRecruiterSize(cJSON *response){
   cJSON *size = cJSON_GetObjectItem(response,"total_jobs");
   return size->valueint;
}
