/*************************************************************************
    > File Name: test.c
    > Author: CandyMonster37
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "process.h"
#include "config.h"

long int calcFileSize(FILE *fp){
	fseek(fp, 0, SEEK_END);
	long int end = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return end;
}


int main(void){
	unsigned char buff[blockSize];
	bzero(buff, blockSize);
	unsigned char outbuff[FILE_MAX_SIZE];

	FILE *fp = fopen("/home/uer1/test/1.png", "rb");
	long int totalSize = calcFileSize(fp);
	long int totalBlocks = totalSize / dataSize;
	int restCont = totalSize % dataSize;
	if(restCont > 0){
		totalBlocks += 1;
	}
	printf("total blocks: %ld\n", totalBlocks);
	int gottenBlocks = 0;
	int blockID = 0;
	bzero(outbuff, sizeof(outbuff));
	int fileBlockLength = 0;
	long int now = 0;
	while(fileBlockLength = fread(buff+dataStart, sizeof(char), dataSize, fp) > 0) {
		now = ftell(fp);
		long int tmpSize = 0;
		if(now == totalSize) {
			tmpSize = restCont;
		} else {
			tmpSize = dataSize;
		}
		long int tmpStart = blockID * dataSize;
		printf("blockID:%d, tmp_size:%ld, tmpStart: %ld\n", blockID, tmpSize, tmpStart);
		memcpy(outbuff+tmpStart, buff+dataStart, tmpSize);
		blockID += 1;
	}
	fclose(fp);

	fp = fopen("./test.png", "wb");
	fwrite(outbuff, sizeof(char), totalSize, fp);
	fclose(fp);


	/* 测试文件长度获取函数*/
	/*FILE *fp = fopen("/home/uer1/test/1.png", "rb");
	if(fp == NULL) {
		printf("Open file failed:%s\n", strerror(errno)); 
	}
	long int totalSize = calcFileSize(fp);

	printf("1.png: %ldBytes\n", totalSize);*/

	/* 测试字段信息存取函数 */
	/* printf("dataSize:%d\n", dataSize);

	int blockID = 2;
	receiveFile fileInfo;
	initFileInfo(fileInfo);
	printf("init: ID: %d,\tSize: %ld\n", fileInfo.fileName, fileInfo.totalSize);
	
	setFileID(buff, 114);
	setFileSize(buff, 32768);
	setBlockNum(buff, 37);
	
	fileInfo.fileName = getFileID(buff);
	fileInfo.totalSize = getFileSize(buff);
	blockID = getBlockNum(buff);
	printf("read: ID: %d,\tSize: %ld,\tblockID: %d\n", fileInfo.fileName, fileInfo.totalSize, blockID);

	*/

	return 0;
}

