#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
//I am the client

#include <iostream>
using namespace std;

#pragma comment (lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    addrinfo* addrResult = NULL;
    addrinfo settings;
    settings.ai_family = AF_INET;
    settings.ai_socktype = SOCK_STREAM;
    settings.ai_protocol = IPPROTO_TCP;
    ZeroMemory( &settings, sizeof(settings) );
    getaddrinfo("127.0.0.1","11111",&settings,&addrResult);
    SOCKET clientsocket = socket(addrResult->ai_family, addrResult->ai_socktype,addrResult->ai_protocol);
    int result;
    do 
    {
        result = connect(clientsocket,addrResult->ai_addr,addrResult->ai_addrlen);
    } while (result == SOCKET_ERROR);
    freeaddrinfo(addrResult);
    const char* message = "hello world";
    cout << "sending message: " << message <<endl;
    send(clientsocket,message,strlen(message),0);
    char buffer[512] = {0};
    recv(clientsocket,(char*)&buffer,sizeof(buffer),0);
    cout << "received message: " << buffer;
    return 0;
}
