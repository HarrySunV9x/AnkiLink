//
// Created by WorkHarry on 2023/12/8.
//

#include "AnkiSocket.h"

AnkiSocket::AnkiSocket() {
    //创建套接字
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket start");
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(50000);  //端口
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Bind %s", strerror(errno));
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Listen %s", strerror(errno));
    listen(serv_sock, 20);
    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    //向客户端发送数据
    char str[] = "http://c.biancheng.net/socket/";
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Write: %s", strerror(errno));
    write(clnt_sock, str, sizeof(str));

    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket Close");
    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
}

AnkiSocket::~AnkiSocket() {

}