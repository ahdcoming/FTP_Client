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
static const char FTP_RETR[] = "RETR";
static const char FTP_PASS[] = "PASS";
static const char FTP_PASV[] = "PASV";
static const char FTP_PORT[] = "PORT";
static const char FTP_TYPE[] = "TYPE";


// modes
static const char FTP_TYPE_ASCII[] = "A";
static const char FTP_TYPE_BINARY[] = "I";

// Response Codes
static const char RES_DELAY[] = "120 ";
static const char RES_START[] = "125 ";
static const char RES_ABOUT_TO_OPEN[] = "150 ";

static const char RES_OKAY[] = "200 ";
static const char RES_NOT_IMPLEMENTED1[] = "202 ";
static const char RES_SERVICE_READY[] = "220 ";
static const char RES_PASV[] = "227 ";
static const char RES_LOGGED_IN[] = "230 ";

static const char RES_PASSWORD_REQ[] = "331 ";

static const char RES_SERVICE_UNAVAIL[] = "421 ";
static const char RES_NOT_FOUND1[] = "450 ";

static const char RES_SYNTAX_ERR1[] = "500 ";
static const char RES_SYNTAX_ERR2[] = "501 ";
static const char RES_NOT_IMPLEMENTED2[] = "502 ";
static const char RES_NOT_ALLOWED[] = "503 ";
static const char RES_NOT_IMPLEMENTED3[] = "504 ";
static const char RES_NOT_LOGGED_IN[] = "530 ";
static const char RES_ACCOUNT_REQUIRED[] = "532 ";
static const char RES_NOT_FOUND2[] = "550 ";

typedef struct {
  int socketFD;// TODO: remove?
  FILE *socket;
  int dataSocketFD; // TODO: remove?
  FILE *dataSocket;
  int listenSockFD;

  FILE *localFile;
    
  char *hostname;
  char *user;
  char *password;
  char *fileName;
  
  int isActive;
  int mode;
  unsigned int port;
} ftp_client;

int ftpDownloadFile(ftp_client *client);

int ftpConnect(ftp_client *client);
int connectToServer(const char *hostname, unsigned int port);
int ftpLogin(ftp_client *client);
int ftpSetMode(ftp_client *client);
int ftpSetBehaviour(ftp_client *client);
int ftpSendDownloadSignal(ftp_client *client);
int ftpReveiveData(ftp_client *client);

#endif /* ftpclient_h */
