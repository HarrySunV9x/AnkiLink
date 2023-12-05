#include <iostream>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

class AnkiSocket{

    public:
    AnkiSocket(char* command){
        initSocket(linkSocket);
        initAddr(sockAddr);
        if(command == "server"){
            serverSocket(linkSocket, sockAddr);
        }
        else{
            clintSocket(linkSocket, sockAddr);
        }
        closesocket(linkSocket);
    }

    ~AnkiSocket(){ 

    }

    private:
    SOCKET linkSocket;
    struct sockaddr_in sockAddr;

    void initSocket(SOCKET& linksocket){
        linkSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    void initAddr(struct sockaddr_in& sockAddr){
        memset(&sockAddr, 0, sizeof(sockAddr));             //每个字节都用0填充
        sockAddr.sin_family = PF_INET;                      //使用IPv4地址
        sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
        sockAddr.sin_port = htons(1234);                    //端口        
    }

    void serverSocket(SOCKET& linksocket, struct sockaddr_in& sockAddr){
        bind(linksocket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        //进入监听状态
        listen(linksocket, 20);
        //接收客户端请求
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        SOCKET clntSock = accept(linksocket, (SOCKADDR*)&clntAddr, &nSize);
        //向客户端发送数据
        char *str = "Hello World!";
        send(clntSock, str, strlen(str)+sizeof(char), NULL);
        //关闭套接字
        closesocket(clntSock);        
    }

    void clintSocket(SOCKET& linksocket, struct sockaddr_in& sockAddr){
        connect(linksocket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        //接收服务器传回的数据
        char szBuffer[MAXBYTE] = {0};
        recv(linksocket, szBuffer, MAXBYTE, NULL);
        //输出接收到的数据
        printf("Message form server: %s\n", szBuffer); 
    }
    
};