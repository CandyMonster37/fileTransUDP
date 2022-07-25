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
#define FILE_MAX_SIZE 327680 // max = 2836100, about 2.7MB

// 通信相关设置
#define MAX_INTERVAL 1000  //最大间隔秒数
#define DELAY_TIME 0.5r  //发送方每发送一个文件块就等待多少秒

// UDP数据报相关设置
#define blockSize 576
#define headSize 20
#define dataSize (blockSize-headSize)

// 控制信息字段大小(Bytes)及偏移
#define fileID 8
#define fileSize 8
#define blockNum 4


#define idxFileID 0
#define idxFileSize (idxFileID+fileID)
#define idxBlockNum (idxFileID+fileID+fileSize)

/* 控制信息字段20B，每个字段8/8/4B。使用20B，保留0B */

// 数据字段偏移
#define dataStart  headSize

#endif


