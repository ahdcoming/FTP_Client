//
//  ftpclient.h
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 1/31/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef ftpclient_h
#define ftpclient_h

#include <stdio.h>

enum {
  BUFFER_SIZE = 1024
};

// FTP commands
static const char FTP_USER[] = "USER";
static const char FTP_PASS[] = "PASS";
static const char FTP_PASV[] = "PASV";
static const char FTP_PORT[] = "PORT";
static const char FTP_TYPE[] = "TYPE";
static const char FTP_OPEN[] = "OPEN";


// modes
static const char FTP_TYPE_ASCII[] = "A";
static const char FTP_TYPE_BINARY[] = "I";

// Reply Codes
static const char RES_DELAY[] = "120 ";
static const char RES_OKAY[] = "200 ";
static const char RES_SERVICE_READY[] = "220 ";
static const char RES_LOGGED_IN[] = "230 ";
static const char RES_PASSWORD_REQ[] = "331 ";



typedef struct {
  int socketFD;
  FILE *file;
  FILE *socket;
  char* user;
  char* password;
  int isAvtive;
  int mode;
} ftp_client;

void ftpConnect(ftp_client *client, const char *hostname, unsigned int port);
int createSocket(const char *hostname, unsigned int port);
int ftpLogin(ftp_client *client);
int ftpSetMode(ftp_client *client);

#endif /* ftpclient_h */
