//
//  argparser.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 1/31/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "argparser.h"
#include "utils.h"

/* Parse a single option. */
error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
   know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  
  switch (key)
  {
    case 'v':
      printf("%s\n", program_version);
      exit(0);
      break;
    case 'h':
      argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
      exit(0);
      break;
    case 'a':
      arguments->isAvtive = 1;
      break;
    case 'f':
      arguments->file = arg;
      break;
    case 's':
      arguments->hostname = arg;
      break;
    case 'p':
      arguments->port = atoi(arg);
      if (arguments->port > 65536) {
        fprintf(stderr, "Invalid port number.\n");
        exit(GENERIC_ERROR);
      }
      break;
    case 'n':
      arguments->user = arg;
      break;
    case 'P':
      arguments->password = arg;
      break;
    case 'm':
      if (strcmp(arg, "ASCII") == 0) {
        arguments->mode = MODE_ASCII;
      }
      else if (strcmp(arg, "binary") == 0) {
        arguments->mode = MODE_BINARY;
      }
      else {
        fprintf(stderr, "Invalid mode. Should be either `ASCII` or `binary`.\n");
        exit(GENERIC_ERROR);
      }
      break;
    case 'b':
      arguments->num_bytes = atoi(arg);
      break;
    case 'l':
      arguments->logIsOn = 1;
      arguments->logfile = arg;
      break;
    case 'w':
      arguments->swarmIsOn = 1;
      arguments->swarm_config_file = arg;
      break;
    case ARGP_KEY_END:
      if (state->argc < 2) {
        /* No inputs. */
        argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
        exit(0);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}