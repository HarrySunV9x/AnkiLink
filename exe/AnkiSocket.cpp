/*
 * 类名称: AnkiSocket
 * 文件名称: AnkiSocket.cpp
 * 创建日期: 2023-12-05
 * 作者: Harry Sun
 * 类描述: 这个类提供了一个用于网络通信的socket的具体实现。
 */

#include "include/AnkiSocket.h"
#include <vector>
#include <commdlg.h> //用于实现windows对话框
#include <iomanip>   //用于控制精度百分比

#define DEFAULT_ADDRESS "127.0.0.1" // 默认的IP地址
#define DEFAULT_PORT 1120           // 默认的端口号

// 错误消息打印函数
void errorMessage(std::string type)
{
    static WSADATA wsaData;
    static int wsaLastError = WSAGetLastError();
    static std::vector<char> errorText(256);

    wsaLastError = WSAGetLastError();
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        wsaLastError,
        0,
        errorText.data(),
        errorText.size(),
        nullptr);
    std::cerr << "Socket " << type << " Error: " << wsaLastError << " - " << errorText.data() << std::endl;
}

static int ret; // 检查错误状态

// 构造函数：初始化socket
AnkiSocket::AnkiSocket()
{
    initSocket();
}

// 析构函数：关闭socket
AnkiSocket::~AnkiSocket()
{
    closesocket(linkSocket);
}

// 设置socket的IP地址
void AnkiSocket::setSocketAddr(char *addr)
{
    sockAddr.sin_addr.s_addr = inet_addr(addr);
    std::cout << "Addr set to :" << addr << std::endl;
}

// 设置socket的端口号
void AnkiSocket::setSocketPort(int port)
{
    sockAddr.sin_port = htons(port);
    std::cout << "Port set to :" << port << std::endl;
}

// 根据命令启动socket作为服务器或客户端
void AnkiSocket::startSocket(char *command)
{
    if (strcmp(command, "server") == 0)
    {
        serverSocket();
    }
    else
    {
        clientSocket();
    }
}

// 使用默认值初始化socket
void AnkiSocket::initSocket()
{
    linkSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // socket对象

    memset(&sockAddr, 0, sizeof(sockAddr));                // 每个字节都用0填充
    sockAddr.sin_family = PF_INET;                         // 使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr(DEFAULT_ADDRESS); // 具体的IP地址
    sockAddr.sin_port = htons(DEFAULT_PORT);               // 端口
}

// 配置并启动服务器socket
void AnkiSocket::serverSocket()
{
    bind(linkSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
    listen(linkSocket, 20);

    // 接受客户端连接
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(linkSocket, (SOCKADDR *)&clntAddr, &nSize);

    char buffer[1024] = {0};
    std::ofstream outfile("received_file.txt", std::ofstream::binary);

    // 打开文件时的错误处理
    if (!outfile)
    {
        std::cerr << "Error: Cannot open 'received_file.txt' for writing." << std::endl;
        closesocket(clntSock);
        return;
    }

    // 从socket读取数据并写入文件
    int valread;
    while ((valread = recv(clntSock, buffer, 1024, 0)) > 0)
    {
        outfile.write(buffer, valread);
    }

    // 读取socket时的错误处理
    if (valread == SOCKET_ERROR)
    {
        errorMessage("Recive");
        return;
    }

    // 数据传输后关闭文件和socket
    outfile.close();
    const char *RECEIPT_CONFIRMATION = "RECEIPT_CONFIRMED";
    send(clntSock, RECEIPT_CONFIRMATION, strlen(RECEIPT_CONFIRMATION) + 1, 0);
    closesocket(clntSock);
}

// 配置并启动客户端socket
void AnkiSocket::clientSocket()
{
    char buffer[1024] = {0};
    const char *END_OF_FILE_MARKER = "FILE_TRANSFER_COMPLETE";
    OPENFILENAME ofn;
    char szFile[260]; // 存储文件名

    // windows 打开文件对话框
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    ret = connect(linkSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
    if (ret == SOCKET_ERROR)
    { // 当返回值为-1，代表连接失败进行错误处理
        errorMessage("Connect");
    }

    // 打开文件
    if (GetOpenFileName(&ofn) == false)
    {
        std::cerr << "File Open Error: File not found or cannot be opened." << std::endl;
        return; // 退出函数
        // 使用ofn.lpstrFile作为文件路径
    }

    std::ifstream infile(ofn.lpstrFile, std::ifstream::binary);
    if (!infile)
    {
        DWORD error = GetLastError();
        std::cerr << "File Open Error: " << error << std::endl;
        return; // 退出函数
    }

    // 设置输入文件流的读取位置到文件末尾
    infile.seekg(0, infile.end);
    // 获取文件的总大小
    long totalSize = infile.tellg();
    // 重新设置文件流的读取位置到文件开始
    infile.seekg(0, infile.beg);
    // 初始化已发送的数据总量为0
    long totalSent = 0;

    // 循环读取文件并发送数据
    while (true)
    {
        // 从文件中读取数据到缓冲区
        infile.read(buffer, sizeof(buffer));
        // 获取实际读取的字节数
        std::streamsize bytes_read = infile.gcount();

        // 如果读取到数据
        if (bytes_read > 0)
        {
            // 通过套接字发送数据
            ret = send(linkSocket, buffer, static_cast<int>(bytes_read), 0);
            // 检查是否发送错误
            if (ret == SOCKET_ERROR)
            {
                // 如果有错误，输出错误消息并返回
                errorMessage("Send");
                return; // 或者处理错误
            }
            // 累加已发送的数据量
            totalSent += bytes_read;

            // 计算并显示发送进度
            double progress = static_cast<double>(totalSent) / totalSize * 100;
            std::cout << "\rProgress: " << std::fixed << std::setprecision(2) << progress << "%" << std::flush;
        }

        // 检查是否到达文件末尾
        if (infile.eof())
        {
            // 到达文件末尾，结束循环
            break;
        }
        // 检查文件读取是否失败
        if (infile.fail())
        {
            // 如果读取失败，输出错误信息并返回
            std::cerr << "Failed to read from file." << std::endl;
            return;
        }
    }

    std::cout << std::endl; // 完成后换行

    // 关闭文件和socket
    infile.close();
    shutdown(linkSocket, SD_SEND);

    // 接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    int ret = recv(linkSocket, szBuffer, MAXBYTE, NULL);
    // 输出接收到的数据
    if (ret == SOCKET_ERROR)
    { // 当返回值为-1，代表连接失败进行错误处理
        errorMessage("Recive");
    }
    else if (ret == 0)
    { // TCP 协议确保数据的可靠传输。在实际应用中，接收到长度为零的数据包通常被视为连接终止的信号。在socket客户端场景下这是不正常的
        std::cout << "Socket Error: Remote close the connect!" << std::endl;
    }
    else
    {
        std::cout << "Message from server: " << szBuffer << std::endl;
    }
}
