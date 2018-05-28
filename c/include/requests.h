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

//basic way to print out results and check if they are pulling correctly.
//printf("%s, %s, %s, %s, %s, %s, %s, %s\n", jobtitle->valuestring, company->valuestring, snippet->valuestring, url->valuestring, postDate->valuestring, relDate->valuestring, city->valuestring, state->valuestring);
struct MemoryStruct {
  char *memory;
  size_t size;
};

void error(const char *msg);

char* concatAPI(char *a, const char *b, char *c);

char* concatEndAPI(char *a, const char *b);

char* concatThree(char *a, char *b, char *c);

//https://curl.haxx.se/libcurl/c/https.html
//https://curl.haxx.se/libcurl/c/getinmemory.html
cJSON* getRequest(char *URL);

void returnIndeed(cJSON *response);

void returnAdzuna(cJSON *response);

void returnAuthentic(cJSON *response);

void returnZipRecruiter(cJSON *response);

int returnIndeedSize(cJSON *response);

int returnAdzunaSize(cJSON *response);

int returnRecruiterSize(cJSON *response);

#endif
