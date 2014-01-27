/**
 * @file
 * @brief This file implements a "very" simple sample client.
 * 
 * The client connects to the server, running at SERVERHOST:SERVERPORT
 * and performs a number of storage_* operations. If there are errors,
 * the client exists.
 */

#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "storage.h"

#define SERVERHOST "localhost"
#define SERVERPORT 1111
#define SERVERUSERNAME "admin"
#define SERVERPASSWORD "dog4sale"
#define TABLE "marks"
#define KEY "ece297"
#define MAXSTRINGSIZE 10
#define LOGGING 1
extern FILE* fileptr;

/**
 * @brief Start a client to interact with the storage server.
 *
 * If connect is successful, the client performs a storage_set/get() on
 * TABLE and KEY and outputs the results on stdout. Finally, it exists
 * after disconnecting from the server.
 */
int main(int argc, char *argv[]) {


	char servername [MAXSTRINGSIZE];
	int port = 0;
	char port_string [MAXSTRINGSIZE];
	char username [MAXSTRINGSIZE];
	char password [MAXSTRINGSIZE];
	int connected = 0;
	int authenticated = 0;
	int status;
	char table [MAXSTRINGSIZE];
	char key [MAXSTRINGSIZE];
	struct storage_record r;
	void *connect;
	int choice;
	char input[MAXSTRINGSIZE];



	fileptr = NULL;
	if (LOGGING == 2){
		time_t rawtime;
	  	struct tm * timeinfo;
	  	char file_name [50];
		time (&rawtime);
		timeinfo = localtime (&rawtime);
		strftime (file_name,50,"Client-%F-%H-%M-%S.log",timeinfo);
		fp = fopen(file_name, "w");
	}







	printf("---------------------\n");
	printf("(1) Connect\n");
	printf("(2) Authenticate\n");
	printf("(3) Get\n");
	printf("(4) Set\n");
	printf("(5) Disconnect\n");
	printf("(6) Exit\n");
	printf("---------------------\n");
	printf("Please enter your selection: ");


	if (scanf("%s", input) == EOF){
		choice = 0;
	}
	else
		choice = atoi(input);
	while (1){
		while ( choice > 6 || choice < 1 || (choice > 1 && choice < 6 && !connected)){
			if (choice > 6 || choice < 1){
			printf("\nInvalid input. Please enter a number between 1 and 6\n");
			printf("---------------------\n");
			printf("(1) Connect\n");
			printf("(2) Authenticate\n");
			printf("(3) Get\n");
			printf("(4) Set\n");
			printf("(5) Disconnect\n");
			printf("(6) Exit\n");
			printf("---------------------\n");
			printf("Please enter your selection: ");
			if (scanf("%s", input) == EOF){
				choice = 0;
			}
			else
				choice = atoi(input);
			}
			else if (choice > 1 && choice < 6 && !connected){
				printf("\nPlease connect first!\n");
				printf("Please enter your selection (1 to connect): ");
						if (scanf("%s", input) == EOF){
							choice = 0;
						}
						else
							choice = atoi(input);
			}

		}

		if (choice == 1){
			printf("\nYou have chosen (1) Connect\n");
			if (connected){
				printf("You have already connected. Please disconnect (5) before reconnecting again.\n");
			}
			else{
				printf("Please input the hostname: ");
				scanf("%s", servername);
				printf("Please input the port: ");
				scanf("%s", port_string);
				port = atoi(port_string);
				while (!port){
					// Not a digit
					printf("Invalid port: Port input must be an integer.\nPlease input the port: ");
					scanf("%s", port_string);
					port = atoi(port_string);
				}


				connect = storage_connect(servername, port);
				if(!connect) {
				    printf("Cannot connect to server @ %s:%d. Error code: %d.\n",
				           servername, port, errno);
				  }
				if (connect){
					connected = 1;
					printf("Connection successful!\n");
				}
			}
		}
		else if (choice == 2 && connected){
			printf("\nYou have chosen (2) Authenticate\n");
			if (authenticated){
				printf("You have already authenticated. Please disconnect (5) before authenticating again.\n");
			}
			else{
				printf("Please input the username: ");
				scanf("%s", username);
				printf("Please input the password: ");
				scanf("%s", password);
				status = storage_auth(username, password, connect);
				if(status != 0) {
				  printf("storage_auth failed with username '%s' and password '%s'. " \
				         "Error code: %d.\n", username, password, errno);
				  storage_disconnect(connect);
				}
				else{
					authenticated = 1;
				  	printf("storage_auth: successful.\n");
				}
			}

		}
		else if (choice == 3 && connected){
			printf("\nYou have chosen (3) Get\n");
			printf("Please input the table: ");
			scanf("%s", table);
			printf("Please input the key: ");
			scanf("%s", key);
			status = storage_get(table, key, &r, connect);
			if(status != 0) {
			  printf("storage_get failed. Error code: %d.\n", errno);
			  storage_disconnect(connect);
			}
			else
			printf("storage_get: the value returned for key '%s' is '%s'.\n",
			       key, r.value);

		}
		else if (choice == 4 && connected){
			printf("\nYou have chosen (4) Set\n");
			strncpy(r.value, "some_value", sizeof r.value);
			printf("Please input the table: ");
			scanf("%s", table);
			printf("Please input the key: ");
			scanf("%s", key);
			status = storage_set(table, key, &r, connect);
			if(status != 0) {
			  printf("storage_set failed. Error code: %d.\n", errno);
			  storage_disconnect(connect);
			}
			else
			printf("storage_set: successful.\n");
		}
		else if (choice == 5 && connected){
			printf("\nYou have chosen (5) Disconnect\n");
			status = storage_disconnect(connect);
			if(status != 0) {
			  printf("storage_disconnect failed. Error code: %d.\n", errno);
			}
			else{
				connected = 0;
				printf("Disconnect successful\n");
			}
		}
		else if (choice == 6){
			if (connected){
				printf("\nPlease disconnect before exiting (5 to disconnect)\n");
			}
			else{
				printf("\nYou have chosen (6) Exit\n");
				printf("Exiting now...\n");
				return 0;
			}
		}
		printf("---------------------\n");
		printf("(1) Connect\n");
		printf("(2) Authenticate\n");
		printf("(3) Get\n");
		printf("(4) Set\n");
		printf("(5) Disconnect\n");
		printf("(6) Exit\n");
		printf("---------------------\n");

		printf("Please enter your selection: ");

		if (scanf("%s", input) == EOF){
			choice = 0;
		}
		else
			choice = atoi(input);



	}



























 /* // Connect to server
  void *conn = storage_connect(SERVERHOST, SERVERPORT);
  if(!conn) {
    printf("Cannot connect to server @ %s:%d. Error code: %d.\n",
           SERVERHOST, SERVERPORT, errno);
    return -1;
  }

  // Authenticate the client.
  //int status = storage_auth(SERVERUSERNAME, SERVERPASSWORD, conn);
  if(status != 0) {
    printf("storage_auth failed with username '%s' and password '%s'. " \
           "Error code: %d.\n", SERVERUSERNAME, SERVERPASSWORD, errno);
    storage_disconnect(conn);
    return status;
  }
  printf("storage_auth: successful.\n");

  // Issue storage_set
  //struct storage_record r;
  strncpy(r.value, "some_value", sizeof r.value);
  status = storage_set(TABLE, KEY, &r, conn);
  if(status != 0) {
    printf("storage_set failed. Error code: %d.\n", errno);
    storage_disconnect(conn);
    return status;
  }
  printf("storage_set: successful.\n");

  // Issue storage_get
  status = storage_get(TABLE, KEY, &r, conn);
  if(status != 0) {
    printf("storage_get failed. Error code: %d.\n", errno);
    storage_disconnect(conn);
    return status;
  }
  printf("storage_get: the value returned for key '%s' is '%s'.\n",
         KEY, r.value);

  // Disconnect from server
  status = storage_disconnect(conn);
  if(status != 0) {
    printf("storage_disconnect failed. Error code: %d.\n", errno);
    return status;
  }

  // Exit*/
  return 0;
}