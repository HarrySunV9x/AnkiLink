#include <iostream>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

class AnkiSocket{

    public:
    AnkiSocket(){
      
    }

    ~AnkiSocket(){ 
        
    }

    private:
    SOCKET linkSocket;
    struct sockaddr_in sockAddr;

    void initSocket(SOCKET& linksocket);
    void initAddr(struct sockaddr_in& sockAddr);

    void serverSocket(SOCKET& linksocket, struct sockaddr_in& sockAddr);

    void clintSocket(SOCKET& linksocket, struct sockaddr_in& sockAddr);
};