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

//How to setup the database
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

void insertIndeedDB(char *jobtitle, char *company, char *city, char *state, char *snippet, char *url, double longitude, double latitude, char *relDate, char *postDate){
   PGconn* conn = connectDB();

   char buffer[1024];
   //int num = snprintf(buffer, sizeof(buffer), "INSERT INTO Indeed( jobtitle, company, city, state, snippet, url, longitude, latitude) VALUES(%s, %s, %s, %s, %s, %s, %lf, %lf);", jobtitle, company, city, state, PQescapeLiteral(conn,snippet,(size_t) strlen(snippet)), url, longitude, latitude);
   int num = snprintf(buffer, sizeof(buffer), "INSERT INTO Indeed( jobtitle, company, city, state, snippet, url, longitude, latitude, relDate, postDate) VALUES(%s, %s, %s, %s, %s, %s, %lf, %lf, %s, %s);",  PQescapeLiteral(conn,jobtitle,(size_t) strlen(jobtitle)), PQescapeLiteral(conn,company,(size_t) strlen(company)), PQescapeLiteral(conn,city,(size_t) strlen(city)), PQescapeLiteral(conn,state,(size_t) strlen(state)), PQescapeLiteral(conn,snippet,(size_t) strlen(snippet)), PQescapeLiteral(conn,url,(size_t) strlen(url)), longitude, latitude, PQescapeLiteral(conn,relDate,(size_t) strlen(relDate)), PQescapeLiteral(conn,postDate,(size_t) strlen(postDate)));
   //printf("%s\n", buffer);
   if (num >sizeof(buffer)){
      fprintf(stderr, "ERROR: Buffer is too small. Increase Buffer size\n");
      exit(1);
   }
   //supresses a notice of the table already existing
   PGresult *response = PQexec(conn, "SET client_min_messages = error;");

   response = PQexec(conn, "CREATE TABLE IF NOT EXISTS Indeed(ID SERIAL PRIMARY KEY, jobtitle VARCHAR(255), company VARCHAR(255), city VARCHAR(50), state VARCHAR(25), snippet VARCHAR(500), url VARCHAR(500), longitude DOUBLE PRECISION, latitude DOUBLE PRECISION, relDate VARCHAR(255), postDate VARCHAR(255));");

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

   response = PQexec(conn, "DELETE FROM indeed a USING (SELECT MIN(ctid) as ctid, snippet, jobtitle FROM indeed GROUP BY snippet, jobtitle HAVING COUNT(*) > 1) b WHERE a.jobtitle = b.jobtitle AND a.snippet = b.snippet AND a.ctid <> b.ctid;");

   if (PQresultStatus(response) != PGRES_COMMAND_OK){
      printf("ERROR: DELETE Command failed.\n");
      PQclear(response);
      do_exit(conn);
   }

   //printf("SUCCESS: INSERT to database completed. \n");
   PQclear(response);
   PQfinish(conn);
}
