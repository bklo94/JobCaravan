#ifndef REQUESTS_H
#define REQUESTS_H

#include "../lib/cJSON.h"
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

//https://curl.haxx.se/libcurl/c/https.html
//https://curl.haxx.se/libcurl/c/getinmemory.html
cJSON* getRequest(char *URL){
   CURL *curl_handle;
   CURLcode res;
   struct MemoryStruct chunk;
   chunk.memory = malloc(1);
   chunk.size = 0;
   cJSON *string, *string2, *array, *test;
   int i = 0;
   curl_global_init(CURL_GLOBAL_ALL);

   curl_handle = curl_easy_init();
   curl_easy_setopt(curl_handle, CURLOPT_URL,URL);
   curl_easy_setopt(curl_handle,CURLOPT_WRITEFUNCTION,writeMemoryCallBack);
   curl_easy_setopt(curl_handle,CURLOPT_WRITEDATA,(void *)&chunk);
   curl_easy_setopt(curl_handle,CURLOPT_USERAGENT,"libcurl-agent/1.0");
   res = curl_easy_perform(curl_handle);
   if(res != CURLE_OK){
      fprintf(stderr, "ERROR: curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
   }
   //How to iterate through a cJSON object
   //https://stackoverflow.com/questions/16900874/using-cjson-to-read-in-a-json-array/16901333
   else{
      test = cJSON_Parse(chunk.memory);
      string = cJSON_GetObjectItem(test,"results");
      array = cJSON_GetArrayItem(string,i);
      string2 = cJSON_GetObjectItem(array,"url");
      printf("%lu bytes retrieved\n",(long)chunk.size);
   }

   curl_easy_cleanup(curl_handle);
   free(chunk.memory);
   curl_global_cleanup();

   return string2;
}

char* concatAPI(char *a, const char *b, char *c){
    size_t len = strlen(a) + strlen(b) + strlen(c);
    char *str = malloc(len + 1);
    strcpy(str,a);
    strcat(str,b);
    strcat(str,c);
    return str;
}

#endif
