//
//  ftpclient.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 1/31/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

//#include <unistd.h>

#include "ftpclient.h"
#include "utils.h"

char buffer[BUFFER_SIZE];

void ftpConnect(ftp_client *client, const char *hostname, unsigned int port) {
  client->socketFD = createSocket(hostname, port);
  client->socket = fdopen(client->socketFD, "w+");
  
  // check connection
  fgets(buffer, sizeof(buffer), client->socket);
  
  // Delay, wait for 220
  if (strncmp(buffer, RES_DELAY, 3) == 0) {
    fgets(buffer, sizeof(buffer), client->socket);
  }
  
  if (strncmp(buffer, RES_SERVICE_READY, 3) != 0) {
    fprintf(stderr, "Error: %s", buffer);
    exit(CANNOT_CONNECT_TO_SERVER);
  }
  
  // read all 220- messages
  while (strncmp(buffer, RES_SERVICE_READY, 4) != 0) {
    fgets(buffer, sizeof(buffer), client->socket);
  }
}

/**
 *  Create a socket for the given infomation.
 *
 *  @param hostname hostname
 *  @param port     port number
 *
 *  @return socket FD on success, otherwise exit(CANNOT_CONNECT_TO_SERVER)
 */
int createSocket(const char *hostname, unsigned int port) {
  
  int socketFD;
  int error;
  
  char service[10];
  sprintf(service, "%d", port);
  
  struct addrinfo *addrInfo;
  
  error = getaddrinfo(hostname, service, NULL, &addrInfo);
  if (error != 0) {
    // can't resolve domain name -> Can't connect to server
    fprintf(stderr, "Cannot resolve hostname(%s) with port(%d).\n", hostname, port);
    exit(CANNOT_CONNECT_TO_SERVER);
  }
  
  socketFD = socket(AF_INET, SOCK_STREAM, 0);
  
//  struct timeval tv;
//  fd_set fdset;
//  
//  tv.tv_sec = 5;
//  tv.tv_usec = 0;
//  
//  if (select(socketFD+1, NULL, &fdset, NULL, &tv) > 0) {
//    int so_error;
//    socklen_t len = sizeof so_error;
//    
//    getsockopt(socketFD, SOL_SOCKET, SO_ERROR, &so_error, &len);
//    
//    if (so_error == 0) {
//      return socketFD;
//    }
//    exit(CANNOT_CONNECT_TO_SERVER);
//  }
//  else {
//    exit(CANNOT_CONNECT_TO_SERVER);
//  }
  
  error = connect(socketFD, addrInfo->ai_addr, addrInfo->ai_addrlen);
  if (error == 0) {
    return socketFD;
  }
  else {
    exit(CANNOT_CONNECT_TO_SERVER);
  }
}

/**
 *  Log in
 *
 *  @param client pointer to ftp_client
 *
 *  @return 0 on success, otherwise exit
 */
int ftpLogin(ftp_client *client) {
  
  // put username
  fprintf(client->socket, "%s %s\r\n", FTP_USER, client->user);
  
  // check response
  fgets(buffer, sizeof(buffer), client->socket);
  
  // not 230
  if (strncmp(buffer, RES_LOGGED_IN, 3) != 0) {
    // 331 needs password
    if (strncmp(buffer, RES_PASSWORD_REQ, 3) == 0) {
      // put password
      fprintf(client->socket, "%s %s\r\n", FTP_PASS, client->password);
      
      // check response
      fgets(buffer, sizeof(buffer), client->socket);
      // not 230
      if (strncmp(buffer, RES_LOGGED_IN, 3) != 0) {
        fprintf(stderr, "Error: %s", buffer);
        exit(AUTHENTICATION_ERROR);
      }
    }
    // not 331
    else {
      fprintf(stderr, "Error: %s", buffer);
      exit(AUTHENTICATION_ERROR);
    }
  }

  // read all 230- messages
  while (strncmp(buffer, RES_LOGGED_IN, 4) != 0) {
    fgets(buffer, sizeof(buffer), client->socket);
  }
  return 0;
}

int setBehaviour(ftp_client *client) {
  // TODO: create a scoket
  if (client->isAvtive) {
    fprintf(client->socket, "%s \r\n", FTP_PORT);
  }
  else {
    
    fprintf(client->socket, "%s \r\n", FTP_PASV);
  }
  return 0;
}

int ftpSetMode(ftp_client *client) {
  if (client->mode == MODE_ASCII) {
    fprintf(client->socket, "%s %s\r\n", FTP_TYPE, FTP_TYPE_ASCII);
  }
  else {
    fprintf(client->socket, "%s %s\r\n", FTP_TYPE, FTP_TYPE_BINARY);
  }
  
  fgets(buffer, sizeof(buffer), client->socket);
  if (strncmp(buffer, RES_OKAY, 3) != 0) {
    fprintf(stderr, "Error: %s", buffer);
    exit(SYNTAX_ERROR);
  }
  return 0;
}
