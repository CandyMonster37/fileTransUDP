/*************************************************************************
    > File Name: server.c
    > Author: CandyMonster37
 ************************************************************************/

#include <stdio.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "config.h"
#include "process.h"

int main(int argc, char *argv[]){

	unsigned char buffer[blockSize];
	unsigned char img[FILE_MAX_SIZE] = {0};
	receiveFile fileInfo;
	initFileInfo(&fileInfo);

	struct sockaddr_in server_addr, from;
	bzero(&server_addr, sizeof(server_addr));  // 效果等同于memset(&server_addr, 0, sizeof(server_addr))
	socklen_t len = sizeof(from);
	bzero(&from, len);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

	int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_socket < 0) {
		printf("Failed to create socket. (Server).\n");
		return -1;
	} else {
		printf("Create Socket Successfully. (Server).\n");
	}
	
	// 设置超时，防止recvfrom（）阻塞
	struct timeval timeout;
	timeout.tv_sec = 1;  //秒
    timeout.tv_usec = 0;  //微秒
    if(setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("Setsockopt failed. (Server).\n");
    }

	int err = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err < 0) {
		printf("Failed to bind Port: %d. (Server).\n", SERVER_PORT);
		return -1;
	} else {
		printf("Bind Port Successfully. (Server).\n");
	}

	printf("Server start running...waiting...\n");
	
	char fileName[FILE_NAME_LENGTH+1];	
    int length = 0;
    memset(buffer, 0, blockSize);

	while(1){
    
		while( length = recvfrom(server_socket, buffer, blockSize, 0, (struct sockaddr*)&from, &len))
	    {  
    	   if (length < 0) { 
				printf("waiting new data...\n");
				break;
			}else {
				if(fileInfo.startReceiveTime == 0){
					fileInfo.startReceiveTime = time(NULL);
				}
				if(fileInfo.fileName == 0){
					fileInfo.fileName = getFileID(buffer);
				}
				if(fileInfo.totalSize == 0){
					fileInfo.totalSize = getFileSize(buffer);
				}
				if(fileInfo.totalBlocks == 0){
					fileInfo.totalBlocks = fileInfo.totalSize / dataSize;
					if(fileInfo.totalSize % dataSize != 0){
						fileInfo.totalBlocks += 1;
					}
				}
				int blockID = getBlockNum(buffer);
				printf("receive a package with ID: %d, Size: %d Bytes. \t", blockID, length);
				long int tmpStart = blockID * dataSize;
				
				memcpy((fileInfo.content)+tmpStart, buffer+dataStart, length-headSize);
				if(memcmp(fileInfo.content+tmpStart, buffer+dataStart, length-headSize)==0){
				    printf("Verification passed\n");
				} else {
				    printf("Verification failed\n");
				}
				
				fileInfo.gottenBlocks += 1;
				printf("blockID:%d, dataSize: %d, gotten blocks:%d. \n", blockID, length, fileInfo.gottenBlocks);
				if(fileInfo.gottenBlocks == fileInfo.totalBlocks){
				    printf("Writing to file...\n");
					//fileInfo.isCompleted = true;
					sprintf(fileName, "%ld%s", fileInfo.fileName, FILE_EXTENSION);
					FILE *fp = fopen(fileName, "wb");
					if(fp == NULL) {
						printf("Open file failed:%s\n", strerror(errno));
						break;
					}
					fwrite(fileInfo.content, sizeof(char), fileInfo.totalSize, fp);
					fclose(fp);
					printf("Successfully get file: %s, fileSize: %ld\n", fileName, fileInfo.totalSize);
					initFileInfo(&fileInfo);
					bzero(fileName, sizeof(fileName));
					break;
				}
				if(time(NULL)-fileInfo.startReceiveTime > MAX_INTERVAL){
					printf("Time out. Give up receiving file:%ld%s\n",fileInfo.fileName, FILE_EXTENSION);
					initFileInfo(&fileInfo);
					bzero(fileName, sizeof(fileName));
				}
				bzero(buffer, blockSize);
			}
        	bzero(buffer, blockSize); 
    	}

	}
	
	close(server_socket);
	return 0;
}
