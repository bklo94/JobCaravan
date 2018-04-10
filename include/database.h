#include "DB.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <libpq-fe.h>

//http://zetcode.com/db/postgresqlc/

void do_exit(PGconn *conn){
   PQfinish(conn);
   exit(1);
}

PGconn *connectDB(){
   PGconn *conn = PQconnectdb(DBVAR);

   if (PQstatus(conn) == CONNECTION_BAD){
      fprintf(stderr, "ERROR: Connection to database failed: %s\n", PQerrorMessage(conn));
      do_exit(conn);
   }
   //printf("SUCCESS: Connection to database established. \n");
   return conn;
}

void insertIndeedDB(char *jobtitle, char *company, char *city, char *state, char *snippet, char *url, double longitutde, double latitude){
   PGconn* conn = connectDB();

   char buffer[1024];
   //int num = snprintf(buffer, sizeof(buffer), "INSERT INTO Indeed( jobtitle, company, city, state, snippet, url, longitutde, latitutde) VALUES(%s, %s, %s, %s, %s, %s, %lf, %lf);", jobtitle, company, city, state, PQescapeLiteral(conn,snippet,(size_t) strlen(snippet)), url, longitutde, latitude);
   int num = snprintf(buffer, sizeof(buffer), "INSERT INTO Indeed( jobtitle, company, city, state, snippet, url, longitutde, latitutde) VALUES(%s, %s, %s, %s, %s, %s, %lf, %lf);",  PQescapeLiteral(conn,jobtitle,(size_t) strlen(jobtitle)), PQescapeLiteral(conn,company,(size_t) strlen(company)), PQescapeLiteral(conn,city,(size_t) strlen(city)), PQescapeLiteral(conn,state,(size_t) strlen(state)), PQescapeLiteral(conn,snippet,(size_t) strlen(snippet)), PQescapeLiteral(conn,url,(size_t) strlen(url)), longitutde, latitude);
   printf("%s\n", buffer);
   if (num >sizeof(buffer)){
      fprintf(stderr, "ERROR: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }

   PGresult *response = PQexec(conn, "CREATE TABLE IF NOT EXISTS Indeed(ID SERIAL PRIMARY KEY, jobtitle VARCHAR(255), company VARCHAR(255), city VARCHAR(50), state VARCHAR(25), snippet VARCHAR(500), url VARCHAR(500), longitutde DOUBLE PRECISION, latitutde DOUBLE PRECISION)");

   if (PQresultStatus(response) != PGRES_COMMAND_OK){
      printf("ERROR: CREATE TABLE Command failed.\n");
      PQclear(response);
      do_exit(conn);
   }

   response = PQexec(conn, buffer);

   if (PQresultStatus(response) != PGRES_COMMAND_OK){
      printf("ERROR: INSERT Command failed.\n");
      PQclear(response);
      do_exit(conn);
   }

   //printf("SUCCESS: INSERT to database completed. \n");
   PQclear(response);
   PQfinish(conn);
}
