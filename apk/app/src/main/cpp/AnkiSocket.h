//
// Created by WorkHarry on 2023/12/8.
//
#include <stdio.h>          // 用于标准输入输出函数，如 printf, scanf
#include <string.h>         // 提供字符串操作函数，如 strcpy, strlen
#include <stdlib.h>         // 包含常用的库函数，如 malloc, free
#include <unistd.h>         // 提供UNIX标准系统调用的接口，如 read, write, close
#include <arpa/inet.h>      // 用于网络地址转换函数，如 inet_pton, inet_ntop
#include <sys/socket.h>     // 提供套接字接口函数和数据结构，如 socket, connect
#include <netinet/in.h>     // 定义IP地址和端口号的结构，如 sockaddr_in
#include <iostream>         // 用于C++标准输入输出流，如 std::cout, std::cin
#include <android/log.h>    // 提供Android日志输出函数，如 __android_log_print
#include <sys/types.h>      // 定义用于系统调用的数据类型，如 pid_t, off_t
#include <cstring>          // 提供C风格字符串处理函数，如 strcpy, strlen
#include <fstream>          // 用于C++文件流操作，如 std::ifstream, std::ofstream


#ifndef APK_ANKISOCKET_H
#define APK_ANKISOCKET_H


class AnkiSocket {

public:
    AnkiSocket();
    ~AnkiSocket();
    void ReceiveSocket();

private:
    int serv_sock;
    struct sockaddr_in serv_addr;
};


#endif //APK_ANKISOCKET_H
