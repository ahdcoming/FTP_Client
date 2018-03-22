//
//  configparser.c
//  FTP Client
//
//  Created by Tyler Weimin Ouyang on 2/12/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdlib.h>

#include "configparser.h"
#include "utils.h"

int parseConfigFile(FILE* configFile, struct config* configs) {
  
  int i = 0;
  while (1) {
    struct config *c = &(configs[i]);
    int parsed = fscanf(configFile,
                        "ftp://%[^:]:%[^@]@%[^/]%s\n",
                        c->userName, c->passWord, c->hostName, c->filePath);
    if (parsed < 1 ) {
      break;
    }
    i++;
    if (parsed != 4) {
      fprintf(stderr, "Failed to parse config file at line %d\n", i);
      exit(GENERIC_ERROR);
    }
  }
  if (i < 1) {
    fprintf(stderr, "Empty config file.\n");
    exit(GENERIC_ERROR);
  }
  return i;
}