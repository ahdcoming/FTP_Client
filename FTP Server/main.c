//
//  main.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 1/30/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

#include "options.h"
#include "argparser.h"

int main (int argc, char **argv) {
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
      printf("--file and --server are required.\n");
      exit(100);
    }
  }
  
  exit (0);
}
