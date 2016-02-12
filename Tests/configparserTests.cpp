//
//  configparserTests.cpp

//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 2/1/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "gtest/gtest.h"
#include <string>
#include <stdio.h>

extern "C" {
#include "configparser.h"
}



namespace {
  TEST(ConfigparserTest, parsesConfigFile) {
    FILE *configFile = fopen("config", "r");
    struct config configs[10];
    int actual = parseConfigFile(configFile, configs);
    EXPECT_EQ(4, actual);
  }
  
}  // namespace