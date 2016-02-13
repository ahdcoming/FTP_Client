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
  
  TEST(UtilsTests, initializeQueue__odd_1) {
    TaskQueue queue;
    initializeQueue(&queue, 2001, 1);
    EXPECT_EQ(0, queue.last->start);
    EXPECT_EQ(2001, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueue__odd_2) {
    TaskQueue queue;
    initializeQueue(&queue, 2001, 2);
    EXPECT_EQ(1000, queue.last->start);
    EXPECT_EQ(1001, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueue__odd_3) {
    TaskQueue queue;
    initializeQueue(&queue, 1999, 3);
    EXPECT_EQ(1332, queue.last->start);
    EXPECT_EQ(667, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueue__even_3) {
    TaskQueue queue;
    initializeQueue(&queue, 2000, 3);
    EXPECT_EQ(1332, queue.last->start);
    EXPECT_EQ(668, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueueB__even_300) {
    TaskQueue queue;
    initializeQueueB(&queue, 2000, 300);
    EXPECT_EQ(1800, queue.last->start);
    EXPECT_EQ(200, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueueB__even_1) {
    TaskQueue queue;
    initializeQueueB(&queue, 200, 1);
    EXPECT_EQ(199, queue.last->start);
    EXPECT_EQ(1, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueueB__biggerSegSize) {
    TaskQueue queue;
    initializeQueueB(&queue, 200, 300);
    EXPECT_EQ(0, queue.last->start);
    EXPECT_EQ(200, queue.last->size);
  }
  
  TEST(UtilsTests, initializeQueueB__multiple) {
    TaskQueue queue;
    initializeQueueB(&queue, 200, 100);
    EXPECT_EQ(100, queue.last->start);
    EXPECT_EQ(100, queue.last->size);
  }
  
  TEST(UtilsTests, dequeue) {
    TaskQueue queue;
    initializeQueueB(&queue, 199, 100);
    Task *task = dequeue(&queue);
    EXPECT_EQ(0, task->start);
    EXPECT_EQ(100, task->size);
    free(task);
    task = dequeue(&queue);
    EXPECT_EQ(100, task->start);
    EXPECT_EQ(99, task->size);
    free(task);
    EXPECT_EQ(NULL, dequeue(&queue));
  }
  
}  // namespace