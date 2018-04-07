#ifndef REQUESTS_H
#define REQUESTS_H

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

void getRequest(char *requests){
   CURL *curl;
   CURLcode res;

   curl_global_init(CURL_GLOBAL_DEFAULT);

   curl = curl_easy_init();
   if (curl){
      curl_easy_setopt(curl, CURLOPT_URL, requests);
#ifdef SKIP_PEER_VERIFICATION
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST,0L);
#endif
   res = curl_easy_perform(curl);
   if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
   curl_easy_cleanup(curl);
   }
   curl_global_cleanup();
   return;
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
