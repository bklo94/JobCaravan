#ifndef REQUESTS_H
#define REQUESTS_H

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

void error(const char *msg) { perror(msg); exit(0); }

struct MemoryStruct {
  char *memory;
  size_t size;
};

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
   cJSON *response;
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
   cJSON *jobtitle, *company, *city, *state, *snippet, *latitude, *longitude, *url, *array;
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
      insertIndeedDB(jobtitle->valuestring, company->valuestring, city->valuestring, state->valuestring, snippet->valuestring, url->valuestring, longitude->valuedouble, latitude->valuedouble);
   }
   printf("%i\n", i);
}

cJSON *returnIndeedSize(cJSON *response){
   cJSON *size = cJSON_GetObjectItem(response,"totalResults");
   //printf("%i\n", cJSON_GetObjectItem(response,"totalResults")->valueint);
   return size;
}

#endif
