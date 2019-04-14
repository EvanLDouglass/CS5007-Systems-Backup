// Modified by Evan Douglass, April 09 2019.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "QueryProtocol.h"
#include "queryclient.h"

char *port_string;
unsigned short int port;
char *ip;

#define BUFFER_SIZE 1500
#define MAX_QUERY_LEN 100

/**
 * This is a helper function that makes a connection with a
 * server running on the given hostname and listening for connections
 * on the given port.
 *
 * This function is adapted from our textbook: chapter 11, section 4.
 *
 * Returns a connection descriptor if successful,
 * -1 with an error message on error.
 */
int open_clientfd(char* hostname, char* port) {
  int clientfd;  // result: the file descriptor of a socket
  struct addrinfo hints, *listp, *p;

  // Get server info
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;  // Used for TCP
  // The following flags I don't fully understand, but I'm trusting the
  // textbook authors to make the right choices here
  hints.ai_flags = AI_NUMERICSERV;  // using a numeric descriptor
  hints.ai_flags |= AI_ADDRCONFIG;  // used for connection
  // Call to get info
  int result = getaddrinfo(hostname, port, &hints, &listp);
  if (result) {
    fprintf(stderr, "Something went wrong getting server info: %s\n",
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
    fprintf(stderr,
        "Couldn't make a connection. Please try running the program again.\n");
    return -1;
  } else {
    // Success, return socket file descriptor
    printf("Connected to %s\n\n", hostname);
    return clientfd;
  }
}

// Prints an error message
void ProtocolError() {
  printf("Protocol Error: Please try again.\n");
}

// Reads from a socket and adds a null terminator to the buffer
// buffLen should be one less than the size of the buffer to avoid
// overwriting any part of the response.
int ReadAddNull(int socket, char* buffer, int buffLen) {
  int len = read(socket, buffer, buffLen);
  // Ensure there is a null terminator on the string
  buffer[len] = '\0';
  return len;
}

void RunQuery(char *query) {
  // Connect to the server
  int sockfd = open_clientfd(ip, port_string);

  /*
   * Do the query-protocol
   * =====================
   */
  char result[BUFFER_SIZE];
  // Server sends ACK after connection
  ReadAddNull(sockfd, result, BUFFER_SIZE-1);
  // Check that protocol is followed
  if (CheckAck(result) == -1) {
    ProtocolError();
    close(sockfd);
    return;
  }

  int numResponses;
  // Write query to server
  write(sockfd, query, strlen(query));
  // Get number of results back
  ReadAddNull(sockfd, result, BUFFER_SIZE-1);
  if (SendAck(sockfd) == -1) {
    ProtocolError();
    close(sockfd);
    return;
  }
  numResponses = atoi(result);
  if (numResponses == 0) {
    puts("No results");
  }

  // Get all the search results
  for (int i = 0; i < numResponses; i++) {
    ReadAddNull(sockfd, result, BUFFER_SIZE-1);
    printf("%s\n\n", result);
    if (SendAck(sockfd) == -1) {
      ProtocolError();
      close(sockfd);
      return;
    }
  }

  // Server sends GOODBY before closing connection
  ReadAddNull(sockfd, result, BUFFER_SIZE-1);
  if (CheckGoodbye(result) == -1) {
    ProtocolError();
    close(sockfd);
    return;
  }

  // Close the connection at end of search
  close(sockfd);
}

void RunPrompt() {
  char input[MAX_QUERY_LEN];

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
  if (argc != 3) {
    printf("Must have two arguments.\n");
    printf("Usage: queryclient <IP address/hostname> <port number>\n");
    return 0;
  }
  ip = argv[1];
  port_string = argv[2];

  // Get info from user
  // function runs query from within
  RunPrompt();

  return 0;
}
