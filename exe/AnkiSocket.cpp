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
    int wsaLastError = WSAGetLastError();
    if (wsaLastError == 0)
    {
        std::cerr << "Unidentified Problem, please check the remote." << std::endl;
        return;
    }

    std::vector<char> errorText(256);
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
    // 创建套接字。PF_INET代表IPv4协议族，SOCK_STREAM表示TCP连接，IPPROTO_TCP指定使用TCP协议
    linkSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (linkSocket == INVALID_SOCKET)
    {
        errorMessage("Socket");
        return;
    }

    memset(&sockAddr, 0, sizeof(sockAddr));                // 确保sockAddr结构被清零。memset用于填充内存块，这里用0填充
    sockAddr.sin_family = PF_INET;                         // 设置套接字地址的家族为IPv4
    sockAddr.sin_addr.s_addr = inet_addr(DEFAULT_ADDRESS); // 设置IP地址。inet_addr将点分十进制的IP地址转换为网络字节顺序
    sockAddr.sin_port = htons(DEFAULT_PORT);               // 设置端口号。htons确保端口号的字节顺序符合网络标准
}

// 服务器socket配置和启动
void AnkiSocket::serverSocket()
{
    // 绑定套接字
    if (bind(linkSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        errorMessage("Bind failed");
        closesocket(linkSocket);
        return;
    }

    // 监听套接字，设置最大等待连接数
    if (listen(linkSocket, 20) == SOCKET_ERROR)
    {
        errorMessage("Listen failed");
        closesocket(linkSocket);
        return;
    }

    // 接受客户端连接请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(linkSocket, (SOCKADDR *)&clntAddr, &nSize);
    if (clntSock == INVALID_SOCKET)
    {
        errorMessage("Accept failed");
        closesocket(linkSocket);
        return;
    }

    // 接收文件名
    char fileNameBuffer[260]; // 假定文件名不会超过260个字符
    int fileNameLength = recv(clntSock, fileNameBuffer, 260, 0);
    if (fileNameLength <= 0)
    {
        std::cerr << "Failed to receive file name." << std::endl;
        closesocket(clntSock);
        return;
    }

    // 文件接收和写入
    std::string receivedFileName = fileNameBuffer;
    std::ofstream outfile(receivedFileName, std::ofstream::binary);
    if (!outfile)
    {
        std::cerr << "Error: Cannot open 'received_file.txt' for writing." << std::endl;
        closesocket(clntSock);
        return;
    }

    // 从socket读取数据并写入文件
    int valread;
    char buffer[1024] = {0};
    while ((valread = recv(clntSock, buffer, 1024, 0)) > 0)
    {
        outfile.write(buffer, valread);
    }

    // 读取socket时的错误处理
    if (valread == SOCKET_ERROR)
    {
        errorMessage("Receive");
        outfile.close();
        closesocket(clntSock);
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
    OPENFILENAME ofn; // Windows文件选择对话框结构
    char szFile[260]; // 存储选中的文件名

    // 初始化文件选择对话框
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

    // 连接到服务器
    if (connect(linkSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        errorMessage("Connect");
        closesocket(linkSocket);
        return;
    }

    // 打开文件选择对话框并获取文件名
    if (GetOpenFileName(&ofn) == false)
    {
        // 使用ofn.lpstrFile作为文件路径
        std::cerr << "File Open Error: File not found or cannot be opened." << std::endl;
        closesocket(linkSocket);
        return;
    }

    // 发送文件名到服务器
    std::string fileName = ofn.lpstrFile; // 获取完整文件路径
    auto lastSlash = fileName.find_last_of("\\/");
    if (lastSlash != std::string::npos)
    {
        fileName = fileName.substr(lastSlash + 1); // 提取文件名
    }
    if (send(linkSocket, fileName.c_str(), fileName.size() + 1, 0) == SOCKET_ERROR)
    { // 发送文件名
        errorMessage("Send");
        closesocket(linkSocket);
        return;
    }
    Sleep(100); // 稍微等待，确保文件名被正确接收

    // 打开并读取文件
    std::ifstream infile(ofn.lpstrFile, std::ifstream::binary);
    if (!infile)
    {
        DWORD error = GetLastError();
        std::cerr << "File Open Error: " << error << std::endl;
        return; // 退出函数
    }

    // 文件传输逻辑
    infile.seekg(0, infile.end);     // 设置输入文件流的读取位置到文件末尾
    long totalSize = infile.tellg(); // 获取文件的总大小
    infile.seekg(0, infile.beg);     // 重新设置文件流的读取位置到文件开始
    long totalSent = 0;              // 初始化已发送的数据总量为0

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
            if (send(linkSocket, buffer, static_cast<int>(bytes_read), 0) == SOCKET_ERROR)
            {
                errorMessage("Send failed");
                infile.close();
                closesocket(linkSocket);
                return;
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
            infile.close();
            closesocket(linkSocket);
            return;
        }
    }

    std::cout << std::endl; // 进度条换行

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
    { // TCP 协议确保数据的可靠传输。在实际应用中，接收到长度为零的数据包通常被视为连接终止的信号。
        // 根据本次应用的场景，对于Windows server来说，在socket客户端场景下这是不正常的。
        // 而Android（Linux）服务器场景，文件在接收完后会在finish帧后直接返回一个ack帧关闭连接，因此可能是正常的
        // 具体原理暂时不过多研究，有时间可考虑在stackoverfloww上提问下~
        // 根据这个现象要注意，Android（Linux）场景下，一次socket只能完成一种行为，无法先接收再确认......
        std::cout << "Socket Recive 0: If your server is not windows, this copy may finished!" << std::endl;
    }
    else
    {
        std::cout << "Message from server: " << szBuffer << std::endl;
    }
}
