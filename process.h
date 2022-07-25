/*************************************************************************
    > File Name: process.h
    > Author: CandyMonster37
 ************************************************************************/

#ifndef __PROCESS_H_
#define __PROCESS_H_

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "config.h"

// 服务器接收文件时的控制块信息
typedef struct file_info{
	long int fileName;
	long int totalSize;
	int totalBlocks;
	int gottenBlocks;
	time_t startReceiveTime;
	unsigned char content[FILE_MAX_SIZE];
	bool isCompleted;
}receiveFile;

void initFileInfo(receiveFile *fileInfo);

long int calcFileSize(FILE *fp);

long int getFileID(unsigned char buff[blockSize]);
void setFileID(unsigned char buff[blockSize], long int id);

long int getFileSize(unsigned char buff[blockSize]);
void setFileSize(unsigned char buff[blockSize], long int size);

int getBlockNum(unsigned char buff[blockSize]);
void setBlockNum(unsigned char buff[blockSize], int blockID);

#endif
