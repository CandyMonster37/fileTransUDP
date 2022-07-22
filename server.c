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
#include "config.h"

#define FILE_MAX_SIZE 327680


int main(int argc, char *argv[]){

	char buffer[blockSize];
	unsigned short img[FILE_MAX_SIZE] = {0};

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

	printf("Server running...waiting...\n");

	while(1){

		char fileName[FILE_NAME_LENGTH];
		time_t now = time(NULL);
		long int name = (long int) now;
		//sprintf(fileName, "%ld%s", name, FILE_EXTENSION);
		
		sprintf(fileName, "test.png");
		FILE *fp = fopen(fileName, "wb");
		if (fp == NULL)  {  
     		printf("File:\t%s Can Not Open To Write!\n", fileName);  
        	exit(1);  
    	}
    	
    	int length = 0;
    	memset(buffer, 0, blockSize);
    
		bool writtenFlag = false;
		bool receiveFlag = false;

		while( length = recvfrom(server_socket, buffer, blockSize, 0, (struct sockaddr*)&from, &len))
	    {  
    	   if (length < 0) { 
				printf("waiting new data...\n");
				receiveFlag = writtenFlag?true:false;
				break;
			}else {
				printf("receive a package with length: %d\n", length);
			}

        	int write_length = fwrite(buffer, sizeof(char), length, fp); 
			printf("Write %d bytes to file: %s\n", write_length, fileName);
        	writtenFlag = true;

        	if (write_length < length) {  
            	writtenFlag = false;
				printf("File: %s Write Failed!\n", fileName);  
            	break;
        	}  
        	bzero(buffer, blockSize); 
    	}
    	if(receiveFlag) {
    	    printf("Receive File:  %s\n", fileName);
			fclose(fp);
			return 0;
		}
    	fclose(fp);
		
		/*err = recvfrom(server_cocket, buffer, blockSize, 0, (struct sockaddr*)&from, sizeof(from));
		if (err < 0) {
			time_t now = time(NULL);
			printf("**%s**  Failed to receive a UDP package.\n", ctime(&now));
		} else {
			char fileName[] = "test.png";
			FILE *fp = fopen(fileName, "w");
			if (fp == NULL)  {  
     		   	printf("File:\t%s Can Not Open To Write!\n", fileName);  
        		exit(1);  
    		}
		    // receive package and process

		}*/
	}
	
	close(server_socket);
	return 0;
}
