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
    ftp_client client, client2;
    char user[40] = "anonymous";
    char passwrod[40] = "user@localhost.localnet";
    char hostname[40] = "ftp.ucsb.edu";
    char filename[90] = "/pub/mirrors/ls-lR.gz";
    char localFilePath[100] = "./";
    
    pthread_t tid[2];

    FTPClientTest() {
      makeLocalFilePath(localFilePath, filename);
      setupFTPClient(&client);
      setupFTPClient(&client2);
    }
    
    virtual ~FTPClientTest() {
      close(client.socketFD);
      close(client.dataSocketFD);
      close(client2.socketFD);
      close(client2.dataSocketFD);
    }
    
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    
    virtual void SetUp() {
      // Code here will be called immediately after the constructor (right
      // before each test).
    }
    
    virtual void TearDown() {
    }
    
    void setupFTPClient(ftp_client *client) {
      client->user = user;
      client->password = passwrod;
      client->hostname = hostname;
      client->filePath = filename;
      client->localFilePath = localFilePath;
      client->port = 21;
      client->logfile = NULL;
    }
    
    void setupFTPConnection(ftp_client *client) {
      ftpConnect(client);
      ftpLogin(client);
      client->isActive = 0;
      ftpSetBehaviour(client);
      client->mode = MODE_BINARY;
      ftpSetMode(client);

      client->sharedLocalFile = fopen(client->localFilePath, "w");
    }
    
    void changeFile(char *newFile) {
      
    }
    
    // Objects declared here can be used by all tests in the test case for Foo.
  };
  
//  TEST_F(FTPClientTest, createSocket__givenDomainName__returnsSocketFD) {
//    int result = connectToServer("ftp.ucsb.edu", 21);
//    EXPECT_NE(-1, result);
//  }
//  
//  TEST_F(FTPClientTest, createSocket__givenIPAddr__returnsSocketFD) {
//    int result = connectToServer("128.111.24.43", 21);
//    EXPECT_NE(-1, result);
//  }
//  
//  TEST_F(FTPClientTest, createSocket__givenInvalidDomainName__exit1) {
//    EXPECT_EXIT(connectToServer("foo", 21),
//                ::testing::ExitedWithCode(CANNOT_CONNECT_TO_SERVER),
//                "Cannot resolve hostname");
//  }
//  
//  TEST_F(FTPClientTest, ftpLogin) {
//    ftpConnect(&client);
//    EXPECT_EQ(0, ftpLogin(&client));
//  }
  
//  TEST_F(FTPClientTest, ftpSetMode__setToBinary) {
//    ftpConnect(&client);
//    client.mode = MODE_BINARY;
//    EXPECT_EQ(0, ftpSetMode(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpSetMode__setToASCII) {
//    ftpConnect(&client);
//    client.mode = MODE_ASCII;
//    EXPECT_EQ(0, ftpSetMode(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpSetBehaviour__setToPASV) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 0;
//    EXPECT_EQ(0, ftpSetBehaviour(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpSetBehaviour__setToACTIVE) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 1;
//    EXPECT_EQ(0, ftpSetBehaviour(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpSendDownloadSignal__passiveMode) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 0;
//    ftpSetBehaviour(&client);
//    
//    EXPECT_EQ(0, ftpSendDownloadSignal(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpSendDownloadSignal__activeMode) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 1;
//    ftpSetBehaviour(&client);
//
//    EXPECT_EQ(0, ftpSendDownloadSignal(&client));
//  }
  
//  TEST_F(FTPClientTest, ftpReceiveData__passive_ascii_Mode) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 0;
//    ftpSetBehaviour(&client);
//    client.mode = MODE_ASCII;
//    ftpSetMode(&client);
//    ftpSendDownloadSignal(&client);
//    EXPECT_EQ(0, ftpReceiveData(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpReceiveData__active_ascii_Mode) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 1;
//    ftpSetBehaviour(&client);
//    client.mode = MODE_ASCII;
//    ftpSetMode(&client);
//    ftpSendDownloadSignal(&client);
//    EXPECT_EQ(0, ftpReceiveData(&client));
//  }
//  
//  TEST_F(FTPClientTest, ftpReceiveData__passive_binary_Mode) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 0;
//    ftpSetBehaviour(&client);
//    client.mode = MODE_BINARY;
//    ftpSetMode(&client);
//    ftpSendDownloadSignal(&client);
//    EXPECT_EQ(0, ftpReceiveData(&client));
//  }
//
//  TEST_F(FTPClientTest, ftpReceiveData__active_binary_Mode) {
//    ftpConnect(&client);
//    ftpLogin(&client);
//    client.isActive = 1;
//    ftpSetBehaviour(&client);
//    client.mode = MODE_BINARY;
//    ftpSetMode(&client);
//    ftpSendDownloadSignal(&client);
//    EXPECT_EQ(0, ftpReceiveData(&client));
//  }
  
//  TEST_F(FTPClientTest, ftpReceivePartialData_singleThread) {
//    setupFTPConnection(&client);
//    
//    client.startByte = 10;
//    ftpSetStartingPoint(&client);
//    ftpSendDownloadSignal(&client);
//    client.downloadSize = 19;
//    
//    EXPECT_EQ(0, ftpReceivePartialData(&client));
//  }
  
//  TEST_F(FTPClientTest, ftpReceivePartialData_multiThreads) {
//    
//    initMutex();
//    
//    client.logfile = stdout;
//    client2.logfile = stdout;
//    client.clientID = 1;
//    client2.clientID = 2;
//    setupFTPConnection(&client);
//    long fileSize = ftpGetSize(&client);
//    long firstPartSize = fileSize / 2;
//
//    client.startByte = 0;
//    client2.startByte = firstPartSize;
//    client.sharedLocalFile = fopen(client.localFilePath, "w+b");
//    client2.sharedLocalFile = client.sharedLocalFile;
//    
//    client.downloadSize = firstPartSize;
//    client2.downloadSize = fileSize - firstPartSize;
//
//    
//    pthread_create(&(tid[0]), NULL, &ftpSwarmDownloadFile, &client);
//    ftpSwarmDownloadFile(&client2);
//    pthread_join(tid[0], NULL);
//    EXPECT_EQ(0, 0);
//  }
}  // namespace