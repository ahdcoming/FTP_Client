//
//  utils.h
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 2/1/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef utils_h
#define utils_h

#define MODE_BINARY 0x100
#define MODE_ASCII  0x101

// Error codes
#define CANNOT_CONNECT_TO_SERVER 1
#define AUTHENTICATION_ERROR 2
#define NOT_FOUND_ERROR 3
#define SYNTAX_ERROR 4
#define NOT_IMPLEMENTED_ERROR 5
#define NOT_ALLOWED_ERROR 6
#define GENERIC_ERROR 7

void handleFTPError(char* errorMessage);
int createListenSocket(int sockFD);
int createConnectionSocket(char* response);
void getIPAddrStr(int socketFD, char IPAddrStr[]);
int acceptConnection(int listenSockFD);

void logServer(FILE *logFile, char* message, int clientID);
void logClient(FILE *logFile, char* message, int clientID);

void initMutex();
void makeLocalFilePath(char *localFilePath, char *remoteFilePath);
size_t writeToLocalFile(const void *data, long int offset, size_t size, FILE *localFile);

#endif /* utils_h */
