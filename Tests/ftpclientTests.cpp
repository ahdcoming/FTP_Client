//
//  ftpclientTests.c
//  FTP Server
//
//  Created by Tyler Weimin Ouyang on 2/1/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

//#include <cstdio>

#include "gtest/gtest.h"

// neede by ip for debugging
//#include <netinet/in.h>
//#include <arpa/inet.h>

extern "C" {
#include "utils.h"
#include "ftpclient.h"
}



namespace {
  using namespace std;
  
  // The fixture for testing class Foo.
  class FTPClientTest : public ::testing::Test {
  protected:
    ftp_client client;
    
    FTPClientTest() {
      char user[] = "anonymous";
      char passwrod[] = " ";
      client.user = user;
      client.password = passwrod;
    }
    
    virtual ~FTPClientTest() {
      close(client.socketFD);
    }
    
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    
    virtual void SetUp() {
      // Code here will be called immediately after the constructor (right
      // before each test).
    }
    
    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }
    
    // Objects declared here can be used by all tests in the test case for Foo.
  };
  
  TEST_F(FTPClientTest, createSocket__givenDomainName__returnsSocketFD) {
    int result = createSocket("ftp.ucsb.edu", 21);
    EXPECT_NE(-1, result);
  }
  
  TEST_F(FTPClientTest, createSocket__givenIPAddr__returnsSocketFD) {
    int result = createSocket("128.111.24.43", 21);
    EXPECT_NE(-1, result);
  }
  
  TEST_F(FTPClientTest, createSocket__givenInvalidDomainName__exit1) {
    EXPECT_EXIT(createSocket("foo", 21),
                ::testing::ExitedWithCode(CANNOT_CONNECT_TO_SERVER),
                "Cannot resolve hostname");
  }
  
  TEST_F(FTPClientTest, ftpLogin) {
    ftpConnect(&client,"ftp.ucsb.edu", 21);
    EXPECT_EQ(0, ftpLogin(&client));
  }
  
  TEST_F(FTPClientTest, ftpSetMode__setToBinary) {
    ftpConnect(&client,"ftp.ucsb.edu", 21);
    client.mode = MODE_BINARY;
    EXPECT_EQ(0, ftpSetMode(&client));
  }
  
  TEST_F(FTPClientTest, ftpSetMode__setToASCII) {
    ftpConnect(&client,"ftp.ucsb.edu", 21);
    client.mode = MODE_ASCII;
    EXPECT_EQ(0, ftpSetMode(&client));
  }
  
}  // namespace