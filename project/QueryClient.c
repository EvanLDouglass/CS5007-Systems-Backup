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
 * This function is adapted from our textbook: chapter 11, section 4.
 *
 * Returns a connection descriptor if successful,
 * -1 on getaddrinfo failure
 * -2 on connection failure
 */
int open_clientfd(char* hostname, int port) {
  int clientfd;  // result: the file descriptor of a socket
  struct addrinfo hints, *listp, *p;

  // Get server info
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;  // Used for TCP
  // The following flags I don't fully understand, but I'm trusting the
  // textbook authors to make the right choices here
  hints.ai_flags = AI_NUMERICSERV;  // using a numeric descriptor
  hints.ai_flags |= AI_ADDRCONFIG;  // used for connections (line is using a bitwise OR)
  // Call to get info
  int result = getaddrinfo(hostname, port, &hints, &listp);
  if (result) {
    fprintf(stderr, "Something went wrong in getaddrinfo: %s\n",
            gai_strerror(result));
    return -1;
  }

  // Walk through returned list to find working connection
  for (p = listp; p; p = p->ai_next) {
    // Make a socket descriptor
    if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      // Socket failed, move to next
      continue;
    }

    // Socket succeeded
    if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
      // Successful connection, exit loop
      break;
    } else {
      // Failed connection, try next in loop
      close(clientfd);
    }
  }

  // Clean memory
  freeaddrinfo(listp);

  if (!p) {
    // All connections failed, p is NULL
    return -2;
  } else {
    // Success, return socket file descriptor
    return clientfd;
  }
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
