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

#include "utils.h"
#include "argparser.h"
#include "ftpclient.h"


void main (int argc, char **argv) {
  struct arguments arguments;
  
  /* Default values. */
  arguments.file = NULL;
  arguments.hostname = NULL;
  arguments.user = "anonymous";
  arguments.password = NULL;
  arguments.logfile = NULL;
  arguments.swarm_config_file = NULL;
  arguments.port = 21;
  arguments.mode = MODE_BINARY;
  arguments.isAvtive = 0;
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
  
  FILE *localFile = fopen(arguments->file, "w");
  FILE *logFile = NULL;
  FILE *configFile = NULL;
  
  if (arguments->logIsOn) {
    if (strcmp(arguments->logFile, "-")) {
      logFile = stdout;
    }
    else {
      logFile = fopen(arguments->logFile, "W");
    }
  }
  
  if (arguments->swarmIsOn) {
    configFile = fopen(arguments->swarm_config_file, "r");
    if (configFile == NULL) {
      fprintf(stderr, "File(%s) does not exist.\n", arguments->swarm_config_file);
      exit(GENERIC_ERROR);
    }
  }
  
  createSocket(arguments.hostname, arguments.port);
  
  fclose(logFile);
  fclose(logFile);
  fclose(configFile);
  exit (0);
}

