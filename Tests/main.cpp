//
//  main.c
//  Tests
//
//  Created by Tyler Weimin Ouyang on 2/1/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "gtest/gtest.h"

int main(int argc, char * argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}