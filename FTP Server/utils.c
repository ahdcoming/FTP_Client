//
//  utils.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 2/1/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "utils.h"
#include "ftpclient.h"

#pragma mark - Error handling

void handleFTPError(char* errorMessage) {
  fprintf(stderr, "Error: %s", errorMessage);
  
  if (strncmp(errorMessage, RES_SERVICE_UNAVAIL, 3) == 0) {
    exit(CANNOT_CONNECT_TO_SERVER);
  }
  else if (strncmp(errorMessage, RES_SYNTAX_ERR1, 3) == 0
           || strncmp(errorMessage, RES_SYNTAX_ERR2, 3) == 0) {
    exit(SYNTAX_ERROR);
  }
  else if (strncmp(errorMessage, RES_NOT_IMPLEMENTED1, 3) == 0
           || strncmp(errorMessage, RES_NOT_IMPLEMENTED2, 3) == 0
           || strncmp(errorMessage, RES_NOT_IMPLEMENTED3, 3) == 0) {
    exit(NOT_IMPLEMENTED_ERROR);
  }
  else if (strncmp(errorMessage, RES_NOT_LOGGED_IN, 3) == 0) {
    exit(AUTHENTICATION_ERROR);
  }
  else if (strncmp(errorMessage, RES_NOT_FOUND1, 3) == 0
           || strncmp(errorMessage, RES_NOT_FOUND2, 3) == 0) {
    exit(NOT_FOUND_ERROR);
  }
  else if (strncmp(errorMessage, RES_NOT_ALLOWED, 3) == 0
           || strncmp(errorMessage, RES_ACCOUNT_REQUIRED, 3) == 0) {
    exit(NOT_ALLOWED_ERROR);
  }
  else {
    exit(GENERIC_ERROR);
  }
}
#pragma mark - Networking

int setupSocket(struct sockaddr_in *sockAddr, in_addr_t ipAddr, int port) {
  int sockFD = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFD < 0) {
    perror("Error: failed to create a socket");
    exit(GENERIC_ERROR);
  }
  bzero(sockAddr, sizeof(*sockAddr));
  sockAddr->sin_family = AF_INET;
  sockAddr->sin_addr.s_addr = ipAddr;
  sockAddr->sin_port = htons(port);
  return sockFD;
}

int createListenSocket(int sockFD) {
  struct sockaddr_in sockAddr;
  socklen_t len = sizeof(sockAddr);
  getsockname(sockFD, (struct sockaddr *)&sockAddr, &len);
  
  sockAddr.sin_port = htons(0);
  
  int listenSockFD = socket(AF_INET, SOCK_STREAM, 0);
  
  if (bind(listenSockFD, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0) {
    perror("Error: Failed to bind a socket.");
    exit(GENERIC_ERROR);
  }
  
  listen(listenSockFD, BUFFER_SIZE);
  return listenSockFD;
}

int createConnectionSocket(char* response) {
  unsigned int h1 = 0, h2 = 0, h3 = 0, h4 = 0,
  p1 = 0, p2 = 0;
  sscanf(response, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
         &h1, &h2, &h3, &h4, &p1, &p2);
  
  char addrStr[20];
  sprintf(addrStr, "%d.%d.%d.%d", h1, h2, h3, h4);
  in_addr_t ipAddr = inet_addr(addrStr);
  
  unsigned int port = p1*256 +p2;
  
  struct sockaddr_in sockAddr;
  int socketFD = setupSocket(&sockAddr, ipAddr, port);
  
  if (connect(socketFD, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == 0) {
    return socketFD;
  }
  else {
    perror("ERROR: Failed to create data connection.");
    exit(CANNOT_CONNECT_TO_SERVER);
  }
}

void getIPAddrStr(int socketFD, char IPAddrStr[]) {
  struct sockaddr_in sockAddr;
  socklen_t len = sizeof(sockAddr);
  getsockname(socketFD, (struct sockaddr *)&sockAddr, &len);
  int p1 = ntohs(sockAddr.sin_port) / 256;
  int p2 = ntohs(sockAddr.sin_port) - p1*256;
  sprintf(IPAddrStr, "%s,%d,%d", inet_ntoa(sockAddr.sin_addr), p1, p2);
  
  for (int i = 0; i < strlen(IPAddrStr); i++) {
    if (IPAddrStr[i] == '.')
      IPAddrStr[i] = ',';
  }
}

int acceptConnection(int listenSockFD) {
  int dataSockFD = accept(listenSockFD, NULL, NULL);
  if (dataSockFD < 0) {
    perror("Error: Failed to accept connection");
    exit(GENERIC_ERROR);
  }
  return dataSockFD;
}

#pragma mark - Logging

void logServer(FILE *logFile, char* message) {
  if (logFile) {
    fprintf(logFile, "S->C: %s", message);
  }
}

void logClient(FILE *logFile, char* message) {
  if (logFile) {
    fprintf(logFile, "C->S: %s", message);
  }
}
