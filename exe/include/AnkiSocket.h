/*
 * 接口名称: AnkiSocket
 * 文件名称: AnkiSocket.h
 * 创建日期: 2023-12-06
 * 作者: Harry Sun
 * 接口描述: Socket接口
 */

#include <iostream>
#include <winsock2.h>
#include <sstream>
#include <fstream>
#include <string>

class AnkiSocket
{

public:
    /**
     * 构造函数
     *
     * 实现socket的初始化
     *
     */
    AnkiSocket();

    ~AnkiSocket();
    /**
     * 修改socket地址
     *
     * @param addr (char*) 赋值地址
     *
     */
    void setSocketAddr(char *addr);

    /**
     * 修改socket端口
     *
     * @param port (char*) 赋值端口
     *
     */
    void setSocketPort(int port);

    /**
     * 启动socket
     *
     * @param command (char*) 启动模式
     */
    void startSocket(char *command);

private:
    SOCKET linkSocket;           // socket对象
    struct sockaddr_in sockAddr; // socket数据

    /**
     * 初始化socket对象与数据
     *
     */
    void initSocket();

    /**
     * 服务端socket
     *
     */
    void serverSocket();

    /**
     * 客户端socket
     *
     */
    void clientSocket();
};