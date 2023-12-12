//
// Created by WorkHarry on 2023/12/8.
//

#include "AnkiSocket.h"

AnkiSocket::AnkiSocket() {
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Starting AnkiSocket");
    //创建套接字
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Socket creation failed: %s", strerror(errno));
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Socket created successfully");

    // 将套接字和IP、端口绑定
    memset(&serv_addr, 0, sizeof(serv_addr));  // 清空地址结构
    serv_addr.sin_family = AF_INET;            // 使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 设置IP地址
    serv_addr.sin_port = htons(50000);         // 设置端口号
}

// 析构函数
AnkiSocket::~AnkiSocket() {
    // 关闭套接字
    close(serv_sock);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Socket closed");
}

// 接收数据的函数
void AnkiSocket::ReceiveSocket() {
    // 绑定套接字
    if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Bind failed: %s", strerror(errno));
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Bind successful");

    // 监听套接字，设置最大等待连接数
    if (listen(serv_sock, 20) == -1) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Listen failed: %s", strerror(errno));
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Listening");

    // 接受客户端连接请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Accept failed: %s", strerror(errno));
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Client connected");

    // 接收文件名
    char fileNameBuffer[260];
    int fileNameLength = recv(clnt_sock, fileNameBuffer, 260, 0);
    if (fileNameLength <= 0) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Failed to receive file name: %s", strerror(errno));
        close(clnt_sock);
        return;
    }
    std::string receivedFileName = fileNameBuffer;
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Received file name");

    // 构造文件的完整路径
    std::string internalPath = "/data/data/com.harry.apk/";
    std::string fullPath = internalPath + receivedFileName;
    std::ofstream outfile(fullPath, std::ofstream::binary);

    // 检查文件是否成功打开
    if (!outfile) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Cannot open file: %s", fullPath.c_str());
        close(clnt_sock);
        return;
    }

    // 读取数据并写入文件
    int valread;
    char buffer[1024] = {0};
    while ((valread = recv(clnt_sock, buffer, 1024, 0)) > 0) {
        outfile.write(buffer, valread);
    }

    // 检查数据接收是否有错误
    if (valread < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "AnkiLink", "Error receiving data: %s", strerror(errno));
        outfile.close();
        close(clnt_sock);
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "File transfer completed");

    // 关闭文件
    outfile.close();
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "File closed");


    // 发送接收确认消息
    const char *RECEIPT_CONFIRMATION = "RECEIPT_CONFIRMED";
    send(clnt_sock, RECEIPT_CONFIRMATION, strlen(RECEIPT_CONFIRMATION) + 1, 0);

    // 关闭客户端套接字
    close(clnt_sock);
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "Client socket closed");


}