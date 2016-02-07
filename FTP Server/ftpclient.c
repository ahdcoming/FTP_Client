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

int ftpDownloadFile(ftp_client *client) {
  ftpConnect(client);
  ftpLogin(client);
  ftpSetBehaviour(client);
  ftpSetMode(client);
  ftpSendDownloadSignal(client);
  ftpReveiveData(client);
  return 0;
}

int ftpConnect(ftp_client *client) {
  client->socketFD = connectToServer(client->hostname, client->port);
  client->socket = fdopen(client->socketFD, "w+");
  
  // check connection
  fgets(buffer, sizeof(buffer), client->socket);
  
  // Delay, wait for 220
  if (strncmp(buffer, RES_DELAY, 3) == 0) {
    fgets(buffer, sizeof(buffer), client->socket);
  }
  
  if (strncmp(buffer, RES_SERVICE_READY, 3) != 0) {
    handleFTPError(buffer);
  }
  
  // read all 220- messages
  while (strncmp(buffer, RES_SERVICE_READY, 4) != 0) {
    fgets(buffer, sizeof(buffer), client->socket);
  }
  return 0;
}

/**
 *  Connect to server with the given infomation.
 *
 *  @param hostname hostname
 *  @param port     port number
 *
 *  @return socket FD on success, otherwise exit(CANNOT_CONNECT_TO_SERVER)
 */
int connectToServer(const char *hostname, unsigned int port) {
  
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
        handleFTPError(buffer);
      }
    }
    // not 331
    else {
      handleFTPError(buffer);
    }
  }

  // read all 230- messages
  while (strncmp(buffer, RES_LOGGED_IN, 4) != 0) {
    fgets(buffer, sizeof(buffer), client->socket);
  }
  return 0;
}

/**
 *  Set the mode to binary or ASCII
 *
 *  @param client pointer to ftp_client
 *
 *  @return 0 on success, otherwise exit
 */
int ftpSetMode(ftp_client *client) {
  if (client->mode == MODE_ASCII) {
    fprintf(client->socket, "%s %s\r\n", FTP_TYPE, FTP_TYPE_ASCII);
  }
  else {
    fprintf(client->socket, "%s %s\r\n", FTP_TYPE, FTP_TYPE_BINARY);
  }
  
  fgets(buffer, sizeof(buffer), client->socket);
  if (strncmp(buffer, RES_OKAY, 3) != 0) {
    handleFTPError(buffer);
  }
  return 0;
}

/**
 *  Set behaviour to active or passive
 *  Establish data connection for passive mode
 *
 *  @param client pointer to ftp_client
 *
 *  @return 0 on success, otherwise exit
 */
int ftpSetBehaviour(ftp_client *client) {
  if (client->isActive) {
    client->listenSockFD = createListenSocket(client->socketFD);
    char IPAddrStr[30];
    getIPAddrStr(client->listenSockFD, IPAddrStr);
    
    fprintf(client->socket, "%s %s\r\n", FTP_PORT, IPAddrStr);
    fgets(buffer, sizeof(buffer), client->socket);
    if (strncmp(buffer, RES_OKAY, 3) != 0) {
      handleFTPError(buffer);
    }
  }
  else {
    fprintf(client->socket, "%s \r\n", FTP_PASV);
    fgets(buffer, sizeof(buffer), client->socket);
    if (strncmp(buffer, RES_PASV, 3) != 0) {
      handleFTPError(buffer);
    }
    client->dataSocketFD = createConnectionSocket(buffer);
  }
  
  return 0;
}

int ftpSendDownloadSignal(ftp_client *client) {
  fprintf(client->socket, "%s %s\r\n", FTP_RETR, client->fileName);
  
  if (client->isActive) {
    fflush(client->socket);
    client->dataSocketFD = acceptConnection(client->listenSockFD);
  }
  client->dataSocket = fdopen(client->dataSocketFD, "r");
  
  fgets(buffer, sizeof(buffer), client->socket);
  if (strncmp(buffer, RES_START, 3) != 0
      && strncmp(buffer, RES_ABOUT_TO_OPEN, 3) != 0) {
    handleFTPError(buffer);
  }
  return 0;
}

int ftpReveiveData(ftp_client *client) {
  bzero(buffer, sizeof(buffer));
  unsigned long int readSize = 0;
  do {
    readSize = fread(buffer, 1, sizeof(buffer), client->dataSocket);
    fwrite(buffer, 1, readSize, client->localFile);
  } while(readSize > 0);
  return 0;
}

int confirmClosing() {

  return 0;
}