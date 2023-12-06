/*
 * 类名称: AnkiSocket
 * 文件名称: AnkiSocket.cpp
 * 创建日期: 2023-12-05
 * 作者: Harry Sun
 * 类描述: Socket具体实现
 */

#include <iostream>
#include <winsock2.h>

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 1120

class AnkiSocket
{

public:
    /**
     * 构造函数
     *
     * 根据command的值，实现server或者client
     *
     * @param command (char*) 命令
     */
    AnkiSocket()
    {
        initSocket();
    }

    ~AnkiSocket()
    {
        closesocket(linkSocket);
    }

    /**
     * 修改socket地址
     *
     * @param addr (char*) 赋值地址
     * 
     */
    void setSocketAddr(char *addr)
    {
        sockAddr.sin_addr.s_addr = inet_addr(addr); // 具体的IP地址
        std::cout << "Addr set to :" << addr << std::endl;
    }

    /**
     * 修改socket端口
     *
     * @param port (char*) 赋值端口
     * 
     */
    void setSocketPort(int port)
    {
        sockAddr.sin_port = htons(port);               // 端口
        std::cout << "Port set to :" << port << std::endl;
    }

    /**
     * 启动socket
     *
     * @param command (char*) 启动模式
     */

    void startSocket(char *command)
    {
        if (strcmp(command, "server") == 0) {
            serverSocket();
        }
        else {
            clientSocket();
        }
    }

private:
    SOCKET linkSocket;           // socket对象
    struct sockaddr_in sockAddr; // socket数据

    /**
     * 初始化socket对象与数据
     *
     */
    void initSocket()
    {
        linkSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // socket对象

        memset(&sockAddr, 0, sizeof(sockAddr));                // 每个字节都用0填充
        sockAddr.sin_family = PF_INET;                         // 使用IPv4地址
        sockAddr.sin_addr.s_addr = inet_addr(DEFAULT_ADDRESS); // 具体的IP地址
        sockAddr.sin_port = htons(DEFAULT_PORT);               // 端口
    }

    void serverSocket()
    {
        bind(linkSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
        // 进入监听状态
        listen(linkSocket, 20);
        // 接收客户端请求
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        SOCKET clntSock = accept(linkSocket, (SOCKADDR *)&clntAddr, &nSize);
        // 向客户端发送数据
        const char *str = "Hello World!";
        send(clntSock, str, strlen(str) + sizeof(char), NULL);
        // 关闭套接字
        closesocket(clntSock);
    }

    void clientSocket()
    {
        connect(linkSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
        // 接收服务器传回的数据
        char szBuffer[MAXBYTE] = {0};
        int ret = recv(linkSocket, szBuffer, MAXBYTE, NULL);
        // 输出接收到的数据
        if(ret == -1){
            std::cout << "Receive nothing!"<< std::endl;
        }
        else{
            std::cout << "Message form server: " << szBuffer << std::endl;
        }
    }
};