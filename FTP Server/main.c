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
#include <libgen.h>

#include "utils.h"
#include "argparser.h"
#include "ftpclient.h"


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
      fprintf(stderr, "Either --file and --server or --swarm is required.\n");
      exit(GENERIC_ERROR);
    }
  }
  char localFilePath[100] = "./";
  strcat(localFilePath, basename(arguments.file));
  
  FILE *logFile = NULL;
  FILE *configFile = NULL;
  
  if (arguments.logIsOn) {
    if (strcmp(arguments.logfile, "-")) {
      logFile = stdout;
    }
    else {
      logFile = fopen(arguments.logfile, "W");
    }
  }
  
  if (arguments.swarmIsOn) {
    configFile = fopen(arguments.swarm_config_file, "r");
    if (configFile == NULL) {
      fprintf(stderr, "Config file(%s) does not exist.\n", arguments.swarm_config_file);
      exit(GENERIC_ERROR);
    }
  }
  
  ftp_client ftpClient;
  ftpClient.hostname = arguments.hostname;
  ftpClient.user = arguments.user;
  ftpClient.password = arguments.password;
  ftpClient.filePath = arguments.file;
  ftpClient.localFilePath = localFilePath;
  ftpClient.isActive = arguments.isActive;
  ftpClient.mode = arguments.mode;
  ftpClient.port = arguments.port;
  ftpDownloadFile(&ftpClient);
  
  fclose(logFile);
  fclose(configFile);
  exit (0);
}

