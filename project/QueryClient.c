// Modified by Evan Douglass, April 09 2019.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "includes/QueryProtocol.h"
#include "QueryClient.h"

char *port_string = "1500";
unsigned short int port;
char *ip = "127.0.0.1";

#define BUFFER_SIZE 1000

/**
 * This is a helper function that makes a connection with a
 * server running on the given hostname and listening for connections
 * on the given port.
 *
 * This function is described in our textbook: chapter 11, section 4.
 *
 * Returns a connection descriptor if successful,
 * -1 on Unix error,
 *  -2 on DNS error
 */
int open_clientfd(char* hostname, int port) {
  int clientfd;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  // Make socket descriptor
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  // Get server's IP and port
  if ((hp = gethostbyname(hostname)) == NULL) {
    return -2;
  }

  // Set socket structures with null values
  bzero((char*)&serveraddr, sizeof(serveraddr));
  // Fill in server's socket address structure
  serveraddr.sin_family = AF_INET;
  bcopy((char*)hp->h_addr_list[0],
        (char*)&serveraddr.sin_addr.s_addr, hp->h_length);
  serveraddr.sin_port = htons(port);  // socket from host to server form

  // Connect with server, if possible
  if (connect(clientfd, (SA*)&serveraddr, sizeof(serveraddr)) < 0) {
    return -1;
  }
  return clientfd;
}

void RunQuery(char *query) {
 
  // Find the address

  // Create the socket

  // Connect to the server

  // Do the query-protocol

  // Close the connection
}

void RunPrompt() {
  char input[BUFFER_SIZE];

  while (1) {
    printf("Enter a term to search for, or q to quit: ");
    scanf("%s", input);

    printf("input was: %s\n", input);

    if (strlen(input) == 1) {
      if (input[0] == 'q') {
        printf("Thanks for playing! \n");
        return;
      }
    }
    printf("\n\n");
    RunQuery(input);
  }
}

int main(int argc, char **argv) {
  // Check/get arguments

  // Get info from user

  // Run Query
  
  return 0;
}
