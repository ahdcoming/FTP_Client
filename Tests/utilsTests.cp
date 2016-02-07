//
//  utilsTests.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 2/1/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "gtest/gtest.h"
#include <string>

extern "C" {
#include "utils.h"
}



namespace {
  
  TEST(UtilsTests, extractFileName_0Level) {
    char src[] = "this.txt";
    char dst[10] = "./";
    makeLocalPath(dst, src);
    EXPECT_EQ(0, strcmp(dst, "./this.txt"));
  }
  
  TEST(UtilsTests, extractFileName_1Level) {
    char src[] = "/this.txt";
    char dst[10] = "./";
    makeLocalPath(dst, src);
    EXPECT_EQ(0, strcmp(dst, "./this.txt"));
  }
  
}  // namespace