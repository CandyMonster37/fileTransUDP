/*************************************************************************
    > File Name: config.h
    > Author: CandyMonster37
 ************************************************************************/

#ifndef __CONFIG_H_
#define __CONFIG_H_
 
// 服务器相关设置
#define SERVER_PORT 8888
#define SERVER_IP "192.168.230.129"

// 文件相关设置
#define FILE_EXTENSION ".png"
#define FILE_NAME_LENGTH 255

// 通信相关设置
#define MAX_INTERVAL 1000  //最大间隔秒数

// UDP数据报相关设置
#define blockSize 81920
#define headSize 8

#define idxTotalSize 0
#define sizeLen 4
#define idxBlockNum 4
#define numLen 4
#define dataStart 8

#endif


