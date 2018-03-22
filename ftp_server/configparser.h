//
//  configparser.h
//  FTP Client
//
//  Created by Tyler Weimin Ouyang on 2/12/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef configparser_h
#define configparser_h

#include <stdio.h>

struct config {
  char userName[50];
  char passWord[50];
  char hostName[50];
  char filePath[100];
};

int parseConfigFile(FILE* configFile, struct config* configs);

#endif /* configparser_h */
