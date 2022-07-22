/*************************************************************************
    > File Name: client.c
    > Author: CandyMonster37
 ************************************************************************/

#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include "config.h"
#include <errno.h>

int main(int argc, char *argv[]){
	int client;
	struct sockaddr_in server;
	unsigned char buff[blockSize];
	char fileName[FILE_NAME_LENGTH + 1];
	socklen_t len = 0;

	client = socket(AF_INET, SOCK_DGRAM, 0);

    if (client < 0)
    {
        printf("Create Socket Failed. (Client)\n");
        exit(1);
    }

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &server.sin_addr);

	bzero(fileName, sizeof(fileName));
	if(argc == 1){
		printf("Please input image name which you want to send: ");
		scanf("%s", fileName);
	} else if (argc == 2){
		strcpy(fileName, argv[1]);
	} else {
		printf("Argument error.\n");
		return -1;
	}
	if(strcmp(fileName+(strlen(fileName)-4), FILE_EXTENSION) != 0){
		printf("File type error. Only support .png.\n");
		return -1;
	}

	FILE *fp = fopen(fileName, "rb");
	if (fp == NULL) {
		printf("File %s cann\'t be read. Please check it.\n", fileName);
		return -1;
	} else{
	    memset(buff, 0, blockSize);
		int fileBlockLength = 0;
		while((fileBlockLength = fread(buff, sizeof(char), blockSize, fp)) > 0){
			if(sendto(client, buff, fileBlockLength, 0, (struct sockaddr*)&server, sizeof(server)) < 0){
				printf("Send file %s failed\n", fileName);
				printf("Error no.%d: %s\n", errno, strerror(errno));
				fclose(fp);
				return -1;
			}
			bzero(buff, sizeof(buff));
		}
		fclose(fp);
		printf("Successfully send file: %s.\n", fileName);
	}

	close(client);

	return 0;
}
