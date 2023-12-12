/*
 * 项目名称: AnkiCard
 * 文件名称: main.cpp
 * 创建日期: 2023-12-05
 * 作者: Harry Sun
 * 项目描述: 跨平台 PC 和 Android 的 Anki 卡片
 *
 * 许可协议: 你可以自由使用、修改和分发这个代码，无需特殊许可。
 *
 * 免责声明: 仅供学习。作者对使用本软件所产生的任何损失或问题概不负责。
 */

#include "include/AnkiSocket.h"

/**
 * 地址检查
 *
 * 检查是否是IPV4类型的地址
 *
 * @param str (const char*) 地址
 * @return (bool) 地址是否是IPV4
 */
bool isIPv4Address(const char *str)
{
    if (str == nullptr)
    {
        return false; // 如果字符串为空指针，则不是合法的IPv4地址
    }

    int count = 0; // 用于计数点的数量
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, '.'))
    {
        int num = std::stoi(token); // 将字符串转换为整数
        if (num < 0 || num > 255)
        {
            return false; // 检查每个数字是否在合法范围内
        }
        count++;
    }

    return (count == 4); // IPv4地址应该有4个数字
}

/**
 * 端口检查
 *
 * 检查是否是有效端口
 *
 * @param str (const char*) 端口
 * @return (bool) 地址是否是有效端口
 */
bool isValidPort(const std::string &str)
{
    if (str.empty())
    {
        return false; // 空字符串不是有效的端口号
    }

    for (char c : str)
    {
        if (!std::isdigit(c))
        {
            return false; // 字符串包含非数字字符，不是有效的端口号
        }
    }

    int port = std::stoi(str);           // 将字符串转换为整数
    return (port >= 0 && port <= 65535); // 检查端口号范围
}

/**
 * 打印用法
 *
 * 检查是否是有效端口
 *
 */
void printUsage()
{
    std::cout << "Usage: AnkiLink [server|client] [IP Address] [Port]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  server: Start the server." << std::endl;
    std::cout << "  client: Start the client." << std::endl;
    std::cout << "  IP Address: Optional. Set the IP address to connect to." << std::endl;
    std::cout << "  Port: Optional. Set the port for communication." << std::endl;
}

/**
 * 主函数
 *
 * 接受server或client的命令，执行相应动作
 *
 * @param argc (int) 命令行参数数量
 * @param argv (char* []) 命令行参数数组
 * @return (int) 程序退出码
 */
int main(int argc, char *argv[])
{

    if (argc < 2 || argc > 4)
    { // 参数检查
        printUsage();
        return 1;
    }

    char *command = argv[1];
    if (strcmp(command, "server") != 0 && strcmp(command, "client") != 0)
    { // 判断条件无法直接用argv[1] == "server"，会比较指针地址而不是值。
        std::cout << "Can't detect the command \"" << argv[1] << "\"" << std::endl;
        printUsage();
        return 1;
    }

    WSADATA wsaData; // 声明wsa数据
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    { // 传入版本、数据，启动wsaData
        std::cout << "WSA Start Up Error!" << std::endl;
        return 1;
    }

    AnkiSocket ankiSocket; // 声明socket

    if (argc > 2)
    {
        if (isIPv4Address(argv[2]))
        {
            ankiSocket.setSocketAddr(argv[2]);
        }
        else
        {
            std::cout << "Input Address Type Error!" << std::endl;
            return 1;
        }
    }

    if (argc > 3)
    {
        if (isValidPort(argv[3]))
        {
            ankiSocket.setSocketPort(std::stoi(argv[3]));
        }
        else
        {
            std::cout << "Input Port Type Error!" << std::endl;
            return 1;
        }
    }

    ankiSocket.startSocket(command);

    return 0;
}