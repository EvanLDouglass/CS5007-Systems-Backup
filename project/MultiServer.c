// Modivied by Evan Douglass, April 13 2019.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>


#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "DocIdMap.h"
#include "htll/Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "FileCrawler.h"

#define BUFFER_SIZE 1000

int Cleanup();

DocIdMap docs;
Index docIndex;

#define SEARCH_RESULT_LENGTH 1500

char movieSearchResult[SEARCH_RESULT_LENGTH];

void sigchld_handler(int s) {
  write(0, "Handling zombies...\n", 20);
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


void sigint_handler(int sig) {
  write(0, "Ahhh! SIGINT!\n", 14);
  Cleanup();
  exit(0);
}

/**
 * This is a helper function to open a listening socket. it is adapted
 * from our textbook in Chapeter 11, part 11.4.9.
 *
 * Returns a socket file descriptor on success, -1 on failure.
 */
int open_listenfd(char* port) {
  struct addrinfo hints, *listp, *p;
  int listenfd, optval = 1;

  // Get a list of server addresses
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  // Accept connections on any IP address
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
  // ... using a port number
  hints.ai_flags = AI_NUMERICSERV;
  getaddrinfo(NULL, port, &hints, &listp);

  // Search list for a port we can bind to
  for (p = listp; p; p = p->ai_next) {
    // Make socket descriptor
    if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      // failed socket if executed
      continue;
    }

    // Removes "Address already in use" error from bind
    // Text states this will allow the server to accept connections immediately
    // after a termination and restart.
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
               (const void*)&optval, sizeof(optval));

    // Bind descriptor to address
    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
      // Successful bind
      break;
    } else {
      // Failure, try next
      close(listenfd);
    }
  }

  freeaddrinfo(listp);
  if (!p) {
    // Was not able to find a socket
    puts("Couldn't find a socket");
    return -1;
  }

  // Turn successful socket into a listening socket
  if (listen(listenfd, 10) < 0) {
    // Failure
    close(listenfd);
    puts("Fail on listen()");
    return -1;
  }
  // Success
  return listenfd;
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

/**
 *  Provides a multi-process solution to serving clients
 */
int HandleConnections(int sock_fd,
                      struct sockaddr_storage their_addr,
                      char* response) {
  // Step 5: Accept connection
  // Fork on every connection.
  return 0;
}

void Setup(char *dir) {
  struct sigaction sa;

  sa.sa_handler = sigchld_handler;  // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("Crawling directory tree starting at: %s\n", dir);
  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(dir, docs);
  printf("Crawled %d files.\n", NumElemsInHashtable(docs));

  // Create the index
  docIndex = CreateIndex();

  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex);
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex->ht));
}

int Cleanup() {
  DestroyOffsetIndex(docIndex);
  DestroyDocIdMap(docs);
  return 0;
}

int main(int argc, char **argv) {
  // Get args
  if (argc != 3) {
    printf("Must have two arguments.\n");
    printf("Usage: queryserver <directory to crawl> <port number>\n");
    return 0;
  }

  char* dir_to_crawl = argv[1];
  Setup(dir_to_crawl);

  // Step 1: get address/port info to open
  char* port = argv[2];

  // The following steps are completed in the helper function
  // Step 2: Open socket
  // Step 3: Bind socket
  // Step 4: Listen on the socket
  int listen_fd = open_listenfd(port);

  // Step 5: Handle clients that connect
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  char response[101];
  int conn_fd;
  while (1) {
    // Make connection
    puts("Waiting for connection...");
    addr_size = sizeof(their_addr);
    conn_fd = accept(listen_fd, (struct sockaddr*)&their_addr, &addr_size);
    if (fork() == 0) {
      close(listen_fd);
      printf("Connected on socket %d\n", conn_fd);

      // Send connection ACK
      if (SendAck(conn_fd) == -1) {
        // Close connection and accept next on fail
        ProtocolError();
        close(conn_fd);
        continue;
      }

      // Get query
      SearchResultIter iter;
      ReadAddNull(conn_fd, response, 100);
      iter = FindMovies(docIndex, response);
      if (iter != NULL) {
        // Send number of results
        int numResults = NumResultsInIter(iter);
        snprintf(response, sizeof(response), "%d", numResults);
        write(conn_fd, response, strlen(response));

        // Get ACK
        ReadAddNull(conn_fd, response, 100);
        if (CheckAck(response) == -1) {
          ProtocolError();
          close(conn_fd);
          continue;
        }

        // Give results to client
        while (1) {
          if (numResults == 0) {
            break;
          }

          // Get and write to client
          struct searchResult movieID;
          SearchResultGet(iter, &movieID);
          CopyRowFromFile(&movieID, docs, movieSearchResult);
          write(conn_fd, movieSearchResult, strlen(movieSearchResult));

          // Check for ACK
          ReadAddNull(conn_fd, response, 100);
          if (CheckAck(response) == -1) {
            ProtocolError();
            break;
          }

          // Check for more results
          if (SearchResultIterHasMore(iter)) {
            SearchResultNext(iter);
          } else {
            DestroySearchResultIter(iter);
            break;
          }
        } 
      } else {
        // No search results
        write(conn_fd, "0", 1);

        // Get ACK
        ReadAddNull(conn_fd, response, 100);
        if (CheckAck(response) == -1) {
          ProtocolError();
          close(conn_fd);
        }
      }
      // Step 6: Close the socket
      SendGoodbye(conn_fd);
      close(conn_fd);  // Child
      exit(0);
    }
    close(conn_fd);  // Parent
  }

  // Got Kill signal
  close(listen_fd);
  Cleanup();

  return 0;
}
