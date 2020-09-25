#ifndef __PRACTICAL__H
#define  __PRACTICAL__H
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>/* for Socket data types */
#include <unistd.h>

#define BUFSIZE 32
#define UDPBUFSIZE 1024

void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);
void HandleTCPClient(int clntSocket);
void PrintSocketAddress(const struct sockaddr *address,FILE *stream);
int SetupTCPClientSocket(const char *host, const char *service);
int SetupTCPServerSocket(const char *service);
int SetupUDPServerSocket(const char *service);
int AcceptTCPConnection(int servSock);
void HandleUDPClient(int clntSocket); // Process client

#endif
