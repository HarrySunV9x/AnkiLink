#include <iostream>
#include <winsock2.h>
#include "AnkiSocket.cpp"

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "Please input the command you need!" << std::endl;
        return 1;
    }
    if(argc > 2){
        std::cout << "The number of command is not supported!" << std::endl;
        return 1;
    }    


    WSADATA wsaData;                                        //定义wsa数据
    if(WSAStartup( MAKEWORD(2, 2), &wsaData) != 0){         //传入版本、数据
        std::cout << "WSA Start Up Error!" << std::endl;
        return 1;
    }          


    if(strcmp(argv[1], "server") == 0){           //直接用argv[1] == "server"不行，会比较指针地址而不是值。
        AnkiSocket ankiSocket("server");
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        AnkiSocket ankiSocket("client");
    }
    else{
        std::cout << "Can't detect the command \"" << argv[1] << "\"" << std::endl;
        return 1;
    }
    


    
    return 0;
}