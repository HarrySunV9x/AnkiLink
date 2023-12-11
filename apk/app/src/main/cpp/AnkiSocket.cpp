//
// Created by WorkHarry on 2023/12/8.
//

#include "AnkiSocket.h"

AnkiSocket::AnkiSocket() {

    //创建套接字
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket start");
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Socket: %s", strerror(errno));
    //将套接字和IP、端口绑定
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(50000);  //端口
}

AnkiSocket::~AnkiSocket() {
    //关闭套接字
    close(serv_sock);
}

void AnkiSocket::ReceiveSocket() {
    bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Bind %s", strerror(errno));
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Listen %s", strerror(errno));
    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "clnt_sock Accept %s", strerror(errno));


    // 接收文件名
    char fileNameBuffer[260]; // 假定文件名不会超过260个字符
    int fileNameLength = recv(clnt_sock, fileNameBuffer, 260, 0);
    if (fileNameLength <= 0) {
        perror("Failed to receive file name");
        close(clnt_sock);
        return;
    }
    std::string receivedFileName = fileNameBuffer;
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "clnt_sock Open File");
    std::string internalPath = "/data/data/com.harry.apk/";
    std::string fullPath = internalPath + receivedFileName;
    std::ofstream outfile(fullPath, std::ofstream::binary);

    // 打开文件时的错误处理

    if (!outfile) {
        __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Error: Cannot open %s",
                            fullPath.c_str());
        close(clnt_sock);
        return;
    }

    // 从socket读取数据并写入文件
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "clnt_sock Write File");
    int valread;
    char buffer[1024] = {0};
    while ((valread = recv(clnt_sock, buffer, 1024, 0)) > 0) {
        outfile.write(buffer, valread);
    }

    // 读取socket时的错误处理
    if (valread < 0) {
        __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Receive error");
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Receive Over");
// 数据传输后关闭文件
    outfile.close();
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "File Closed");

// 发送确认消息
    const char *RECEIPT_CONFIRMATION = "RECEIPT_CONFIRMED";
    send(clnt_sock, RECEIPT_CONFIRMATION, strlen(RECEIPT_CONFIRMATION) + 1, 0);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "clnt_sock Send %s", strerror(errno));
// 关闭套接字
    close(clnt_sock);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Socket Closed");

}