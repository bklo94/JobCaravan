#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>

//check for the file for travis-ci
#if __has_include(<libpq-fe.h>)
#include <libpq-fe.h>
#endif
#if __has_include(<postgresql/libpq-fe.h>)
#include <postgresql/libpq-fe.h>
#endif

//How to setup the database
//http://zetcode.com/db/postgresqlc/

void do_exit(PGconn *conn);

PGconn *connectDB();

void insertIndeedDB(char *jobtitle, char *company, char *city, char *state, char *snippet, char *url, double longitude, double latitude, char *relDate, char *postDate);

void insertAdzunaDB(char *jobtitle, char *company, char *city, char *state, char *snippet, char *url, double longitude, double latitude, char *postDate);

void insertAuthenticDB(char *jobtitle, char *company, char *city, char *state, char *snippet, char *url, double longitude, double latitude, char *postDate);

void insertZipDB(char *jobtitle, char *company, char *city, char *state, char *snippet, char *url, double longitude, double latitude, char *relDate, char *postDate);


#endif
