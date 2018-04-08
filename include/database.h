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

void connectDB(){
   PGconn *conn = PQconnectdb(DBVAR);

   if (PQstatus(conn) == CONNECTION_BAD){
      fprintf(stderr, "ERROR: Connection to database failed: %s\n", PQerrorMessage(conn));
      do_exit(conn);
   }

   char *user = PQuser(conn);
   char *db_name = PQdb(conn);
   char *pswd = PQpass(conn);

   printf("User: %s\n", user);
   printf("Database name: %s\n", db_name);
   printf("Password: %s\n", pswd);

   PQfinish(conn);
}
