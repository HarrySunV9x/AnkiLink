//
// Created by WorkHarry on 2023/12/8.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <android/log.h>

#ifndef APK_ANKISOCKET_H
#define APK_ANKISOCKET_H


class AnkiSocket {

public:
    AnkiSocket();
    ~AnkiSocket();
};


#endif //APK_ANKISOCKET_H
