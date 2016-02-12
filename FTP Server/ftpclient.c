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
#include <stdarg.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <pthread.h>

#include "ftpclient.h"
#include "utils.h"


pthread_t tid[1];

#pragma mark - Helpers
void sendMessage (ftp_client *client, const char * format, ...) {
  va_list args;
  va_start (args, format);
  vsprintf (client->buffer,format, args);
  va_end (args);
  fprintf(client->socket, "%s", client->buffer);
  logClient(client->logfile, client->buffer, client->clientID);
}

void receiveMessage(ftp_client *client) {
  fgets(client->buffer, sizeof(client->buffer), client->socket);
  logServer(client->logfile, client->buffer, client->clientID);
}


void *ftpReceiveDataWraper(void *clientvoid) {
  ftpReceiveData((ftp_client *)clientvoid);
  pthread_exit(NULL);
}

int setModeTo(ftp_client *client, int mode) {
  if (mode == MODE_ASCII) {
    sendMessage(client, "%s %s\r\n", FTP_TYPE, FTP_TYPE_ASCII);
  }
  else {
    sendMessage(client, "%s %s\r\n", FTP_TYPE, FTP_TYPE_BINARY);
  }
  
  receiveMessage(client);
  if (strncmp(client->buffer, RES_OKAY, 3) != 0) {
    handleFTPError(client->buffer);
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
    fprintf(stderr, "Cannot connect to server:%s.\n", hostname);
    exit(CANNOT_CONNECT_TO_SERVER);
  }
}

#pragma mark - FTP functions

int ftpDownloadFile(ftp_client *client) {
  ftpConnect(client);
  ftpLogin(client);
  ftpSetBehaviour(client);
  ftpSetMode(client);
  ftpSendDownloadSignal(client);
  if (pthread_create(&tid[0], NULL, &ftpReceiveDataWraper, client)) {
    fprintf(stderr, "Error creating thread\n");
    exit(GENERIC_ERROR);
  }
  ftpConfirmCompletion(client);
  if(pthread_join(tid[0], NULL)) {
    fprintf(stderr, "Error joining thread\n");
    exit(GENERIC_ERROR);
  }
  return 0;
}

void *ftpSwarmConnection(void *clientvoid) {
  ftp_client *client = clientvoid;
  ftpConnect(client);
  ftpLogin(client);
  ftpSetBehaviour(client);
  ftpSetMode(client);
  pthread_exit(NULL);
}

void *ftpSwarmDownloadFile(void *clientvoid) {
  ftp_client *client = clientvoid;
  ftpSetStartingPoint(client);
  ftpSendDownloadSignal(client);
  ftpReceivePartialData(client);
  pthread_exit(NULL);
}

int ftpConnect(ftp_client *client) {
  client->socketFD = connectToServer(client->hostname, client->port);
  client->socket = fdopen(client->socketFD, "w+");
  
  // check connection
  receiveMessage(client);
  
  // Delay, wait for 220
  if (strncmp(client->buffer, RES_DELAY, 3) == 0) {
    receiveMessage(client);
  }
  
  if (strncmp(client->buffer, RES_SERVICE_READY, 3) != 0) {
    handleFTPError(client->buffer);
  }
  // TODO: find a way to generalize this check
  // read all 220- messages
  while (strncmp(client->buffer, RES_SERVICE_READY, 4) != 0) {
    receiveMessage(client);
  }
  return 0;
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
  sendMessage(client, "%s %s\r\n", FTP_USER, client->user);
  
  // check response
  receiveMessage(client);
  
  // not 230
  if (strncmp(client->buffer, RES_LOGGED_IN, 3) != 0) {
    // 331 needs password
    if (strncmp(client->buffer, RES_PASSWORD_REQ, 3) == 0) {
      // put password
      sendMessage(client, "%s %s\r\n", FTP_PASS, client->password);
      
      // check response
      receiveMessage(client);
      // not 230
      if (strncmp(client->buffer, RES_LOGGED_IN, 3) != 0) {
        handleFTPError(client->buffer);
      }
    }
    // not 331
    else {
      handleFTPError(client->buffer);
    }
  }

  // read all 230- messages
  while (strncmp(client->buffer, RES_LOGGED_IN, 4) != 0) {
    receiveMessage(client);
  }
  return 0;
}

/**
 *  You have to call this before `ftpSetMode`.
 *
 *  @param client pointer to ftp_client
 *
 *  @return size of file in bytes
 */
long ftpGetSize(ftp_client *client) {
  long size = 0;
  setModeTo(client, MODE_BINARY);
  sendMessage(client, "%s %s\r\n", FTP_SIZE, client->filePath);
  receiveMessage(client);
  if (strncmp(client->buffer, RES_SIZE, 3) != 0) {
    handleFTPError(client->buffer);
  }
  sscanf(client->buffer, "213 %ld", &size);
  return size;
}

/**
 *  Set the mode to binary or ASCII
 *
 *  @param client pointer to ftp_client
 *
 *  @return 0 on success, otherwise exit
 */
int ftpSetMode(ftp_client *client) {
  return setModeTo(client, client->mode);
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
    
    sendMessage(client, "%s %s\r\n", FTP_PORT, IPAddrStr);
    receiveMessage(client);
    if (strncmp(client->buffer, RES_OKAY, 3) != 0) {
      handleFTPError(client->buffer);
    }
  }
  else {
    sendMessage(client, "%s \r\n", FTP_PASV);
    receiveMessage(client);
    if (strncmp(client->buffer, RES_PASV, 3) != 0) {
      handleFTPError(client->buffer);
    }
    client->dataSocketFD = createConnectionSocket(client->buffer);
  }
  
  return 0;
}

/**
 *  Only works for binary mode
 *
 *  @param client pointer to ftp_client
 *
 *  @return 0 on success, otherwise exit
 */
int ftpSetStartingPoint(ftp_client *client) {
  sendMessage(client, "%s %d\r\n", FTP_REST, client->startByte);
  receiveMessage(client);
  if (strncmp(client->buffer, RES_ACTION_REQ, 3) != 0) {
    handleFTPError(client->buffer);
  }
  return 0;
}

int ftpSendDownloadSignal(ftp_client *client) {
  sendMessage(client, "%s %s\r\n", FTP_RETR, client->filePath);
  receiveMessage(client);
  if (strncmp(client->buffer, RES_START, 3) != 0
      && strncmp(client->buffer, RES_ABOUT_TO_OPEN, 3) != 0) {
    handleFTPError(client->buffer);
  }
  
  if (client->isActive) {
    client->dataSocketFD = acceptConnection(client->listenSockFD);
  }
  client->dataSocket = fdopen(client->dataSocketFD, "r");
  return 0;
}

int ftpReceiveData(ftp_client *client) {
  char data[BUFFER_SIZE] = {0};
  unsigned long int readSize = 0;
  FILE *localFile = fopen(client->localFilePath, "w");
  do {
    readSize = fread(data, 1, sizeof(data), client->dataSocket);
    fwrite(data, 1, readSize, localFile);
  } while(readSize > 0 && ferror(client->dataSocket) == 0);
  
  if (feof(client->dataSocket) == 0) {
    fprintf(stderr, "Download failed.\n");
    exit(GENERIC_ERROR);
  }
  
  close(client->dataSocketFD);
  if (client->listenSockFD > 0) close(client->listenSockFD);
  fclose(localFile);
  return 0;
}

int ftpReceivePartialData(ftp_client *client) {
  char data[BUFFER_SIZE] = {0};
  unsigned long int readSize = 0,
  bytesToRead = client->downloadSize;
  do {
    readSize = fread(data, 1, sizeof(data), client->dataSocket);
    unsigned long int writeSize = bytesToRead < readSize ? bytesToRead : readSize;
    long offset = client->startByte + client->downloadSize - bytesToRead;
    writeToLocalFile(data, offset, writeSize, client->sharedLocalFile);
    bytesToRead -= writeSize;
  } while(readSize > 0
          && ferror(client->dataSocket) == 0
          && bytesToRead > 0);
  
  if (ferror(client->dataSocket) != 0) {
    fprintf(stderr, "Download failed for range %ld + %ld.\n", client->startByte, client->downloadSize);
    exit(GENERIC_ERROR);
  }
  
  ftpStopDownload(client);
  
  if (client->listenSockFD > 0) close(client->listenSockFD);
  return 0;
}

int ftpStopDownload(ftp_client *client) {
  sendMessage(client, "%s \r\n", FTP_ABOR);
  close(client->dataSocketFD);
  receiveMessage(client);
  
  if (strncmp(client->buffer, RES_CONNECTION_ABORTED, 3) == 0
      || strncmp(client->buffer, RES_BROKEN_PIPE, 3) == 0) {
    receiveMessage(client);
  }
  
  if (strncmp(client->buffer, RES_CLOSING_DATA_CONNECTION, 3) != 0) {
    handleFTPError(client->buffer);
  }
  
  return 0;
}

int ftpConfirmCompletion(ftp_client *client) {
  receiveMessage(client);
  if (strncmp(client->buffer, RES_CLOSING_DATA_CONNECTION, 3) != 0
      && strncmp(client->buffer, RES_COMPLETED, 3) != 0) {
    handleFTPError(client->buffer);
  }
  close(client->socketFD);
  return 0;
}
