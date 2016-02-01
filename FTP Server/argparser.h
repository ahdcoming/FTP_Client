//
//  argparser.h
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 1/31/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef argparser_h
#define argparser_h

#include <argp.h>

static char *program_version = "mftp v0.1 Weimin Ouyang";

/* Program documentation. */
static char doc[] = "A FTP client implmented in C for CS176B";

/* The options we understand. */
static struct argp_option options[] = {
  {"version", 'v', 0,          OPTION_ALIAS, "Print program version"},
  {"help",    'h', 0,          0, "Give this help list"},
  {"active",  'a', 0,          0, "Forces active behavior (the server opens the data connection to the client) (default behavior: passive behavior)"},
  {"file",    'f', "file",     0, "Specifies the file to download" },
  {"server",  's', "hostname", 0, "Specifies the server to download the file from" },
  {"port",    'p', "port",     0, "Specifies the port to be used when contacting the server (default value: 21)" },
  {"username",'n', "user",     0, "Uses the username user when logging into the FTP server (default value: anonymous)"},
  {"password",'P', "password", 0, "Uses the password password when logging into the FTP server (default value: user@localhost.localnet)"},
  {"mode",    'm', "mode",     0, "Specifies the mode to be used for the transfer (ASCII or binary) (default value: binary)"},
  {0,         'b', "num-bytes",0, "This option forces each thread to download in segments of size num-bytes."},
  {"log",     'l', "logfile",  0, "Logs all the FTP commands exchanged with the server and the corresponding replies to file logfile. "
    "If the filename is \"-\" then the commands are printed to the standard output."},
  {"swarm",   'w', "swarm-config-file", 0, "Specifies the login, password, hostname and absolute path to the file"},
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *file;
  char *hostname;
  char *user;
  char *password;
  char *logfile;
  char *swarm_config_file;
  unsigned int port;
  unsigned int mode;
  unsigned int isAvtive;
  unsigned int logIsOn;
  unsigned int swarmIsOn;
  unsigned int num_bytes;
};

error_t parse_opt (int key, char *arg, struct argp_state *state);

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, doc };

#endif /* argparser_h */
