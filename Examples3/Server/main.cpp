#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
//I am the server

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
    SOCKET listensocket = socket(addrResult->ai_family, addrResult->ai_socktype,addrResult->ai_protocol);
    bind(listensocket,addrResult->ai_addr,addrResult->ai_addrlen);
    listen(listensocket,SOMAXCONN);
    freeaddrinfo(addrResult);
    SOCKET newClient = accept(listensocket,NULL,NULL);
    closesocket(listensocket);
    char buffer[512] = {0};
    while(0 > recv(newClient,(char*)&buffer,sizeof(buffer),0)){};
    cout << "received message: " << buffer <<endl;
    const char* message = "goodbye world";
    send(newClient,message,strlen(message),0);
    return 0;
}