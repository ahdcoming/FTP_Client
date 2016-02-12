//
//  main.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 1/30/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>
#include <pthread.h>

#include "utils.h"
#include "argparser.h"
#include "ftpclient.h"
#include "configparser.h"


int main (int argc, char **argv) {
  struct arguments arguments;

  /* Default values. */
  arguments.file = NULL;
  arguments.hostname = NULL;
  arguments.user = "anonymous";
  arguments.password = "user@localhost.localnet";
  arguments.logfile = NULL;
  arguments.swarm_config_file = NULL;
  arguments.port = 21;
  arguments.mode = MODE_BINARY;
  arguments.isActive = 0;
  arguments.logIsOn = 0;
  arguments.swarmIsOn = 0;
  arguments.num_bytes = 0;
  
  /* Parse our arguments; every option seen by parse_opt will
   be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  
  if (!arguments.file || !arguments.hostname) {
    if (!arguments.swarmIsOn) {
      fprintf(stderr, "Either --file and --server, or --swarm is required.\n");
      exit(GENERIC_ERROR);
    }
  }
  char localFilePath[100] = "./";
  
  FILE *logFile = NULL;
  FILE *configFile = NULL;
  
  if (arguments.logIsOn) {
    if (strcmp(arguments.logfile, "-") == 0) {
      logFile = stdout;
    }
    else {
      logFile = fopen(arguments.logfile, "w");
    }
  }
  
  if (!arguments.swarmIsOn) {
    makeLocalFilePath(localFilePath, arguments.file);
    ftp_client ftpClient;
    ftpClient.hostname = arguments.hostname;
    ftpClient.user = arguments.user;
    ftpClient.password = arguments.password;
    ftpClient.filePath = arguments.file;
    ftpClient.logfile = logFile;
    ftpClient.localFilePath = localFilePath;
    ftpClient.isActive = arguments.isActive;
    ftpClient.mode = arguments.mode;
    ftpClient.port = arguments.port;
    ftpDownloadFile(&ftpClient);
  }
  else {
    initMutex();
    configFile = fopen(arguments.swarm_config_file, "r");
    if (configFile == NULL) {
      fprintf(stderr, "Config file(%s) does not exist.\n", arguments.swarm_config_file);
      exit(GENERIC_ERROR);
    }
    struct config configs[30];
    int serverCount = parseConfigFile(configFile, configs);
    ftp_client clients[serverCount];
    
    pthread_t tid[30];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for (int i = 0; i < serverCount; i++) {
      ftp_client *c = &clients[i];
      c->user = configs[i].userName;
      c->password = configs[i].passWord;
      c->hostname = configs[i].hostName;
      c->filePath = configs[i].filePath;
      c->logfile = logFile;
      c->mode = MODE_BINARY;
      c->isActive = arguments.isActive;
      c->port = arguments.port;
      c->clientID = i;
      if (pthread_create(&tid[i], &attr, &ftpSwarmConnection, c)) {
        fprintf(stderr, "Error creating thread\n");
        exit(GENERIC_ERROR);
      }
    }
    pthread_join(tid[0], NULL);
    long fileSize = ftpGetSize(clients);
    long segSize = fileSize / serverCount;
    long offset = 0;
    makeLocalFilePath(localFilePath, configs[0].filePath);
    FILE *sharedLocalFile = fopen(localFilePath, "wb");
    
    for (int i = 0; i < serverCount; i++) {
      clients[i].sharedLocalFile = sharedLocalFile;
      clients[i].startByte = offset;
      clients[i].downloadSize = segSize;
      if (i == serverCount - 1) {
        clients[i].downloadSize = fileSize - offset;
      }
      offset += segSize;
      if (i !=0 && pthread_join(tid[i], NULL)) {
        fprintf(stderr, "Error joining thread\n");
        exit(GENERIC_ERROR);
      }
      if (pthread_create(&tid[i], &attr, &ftpSwarmDownloadFile, &clients[i])) {
        fprintf(stderr, "Error creating thread\n");
        exit(GENERIC_ERROR);
      }
    }
    pthread_attr_destroy(&attr);
    for (int i = 0; i < serverCount; i++) {
      if (pthread_join(tid[i], NULL)) {
        fprintf(stderr, "Error joining thread\n");
        exit(GENERIC_ERROR);
      }
    }
  }
  fclose(logFile);
  fclose(configFile);
  exit (0);
}

