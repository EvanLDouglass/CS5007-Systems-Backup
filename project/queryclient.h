#ifndef QUERY_CLIENT_H

#define QUERY_CLIENT_H

int open_clientfd(char* hostname, char* port);
//void RunQueryHelper(int infd, int outfd, const char *query); 
void RunPrompt();

#endif
