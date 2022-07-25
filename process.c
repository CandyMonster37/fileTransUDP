/*************************************************************************
    > File Name: test.c
    > Author: CandyMonster37
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "process.h"
#include "config.h"

void initFileInfo(receiveFile *fileInfo){
	/* 初始化服务器接收文件时的控制块信息 */
	(*fileInfo).fileName = 0;
	(*fileInfo).totalSize = 0;
	(*fileInfo).totalBlocks = 0;
	(*fileInfo).gottenBlocks = 0;
	(*fileInfo).isCompleted = false;
	(*fileInfo).startReceiveTime = 0;
	bzero(&((*fileInfo).content), sizeof((*fileInfo).content));
}


/* 获取文件大小 */
long int calcFileSize(FILE *fp){
	fseek(fp, 0, SEEK_END);
	long int end = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return end;
}

/* 从接收到的buffer中获取相关字段信息 / 向buffer中写入相关字段的信息 */

long int getFileID(unsigned char buff[blockSize]){
	long int id = 0;
	memcpy(&id, buff + idxFileID, fileID);
	return id;
}

void setFileID(unsigned char buff[blockSize], long int id){
    memcpy(buff + idxFileID, &id, fileID);
}

long int getFileSize(unsigned char buff[blockSize]){
    long int size = 0;
    memcpy(&size, buff + idxFileSize, fileSize);
    return size;
}

void setFileSize(unsigned char buff[blockSize], long int size){
    memcpy(buff + idxFileSize, &size, fileSize);
}

int getBlockNum(unsigned char buff[blockSize]){
    int blockID = 0;
    memcpy(&blockID, buff + idxBlockNum, blockNum);
    return blockID;
}

void setBlockNum(unsigned char buff[blockSize], int blockID){
    memcpy(buff + idxBlockNum, &blockID, blockNum);
}
