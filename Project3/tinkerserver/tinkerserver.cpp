//simple tuple example (variadic templates)
#include <iostream>
#include <string>
#include <sstream>
#include <ws2tcpip.h>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

struct X
{
	X()
	{
		//init winsock
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	~X()
	{
		WSACleanup();
	}
	void serverside();
	void clientside();
};

int main()
{
	X x;
	std::thread t1(&X::serverside, x);
	t1.detach();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::thread t2(&X::clientside, x);
	t2.detach();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("pause");
}


void X::serverside()
{
	//make a socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "bad socket";
	}

	//construct bind address
	sockaddr_in sa{ 0 };
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(5150);
	inet_pton(AF_INET, "127.0.0.1", &(sa.sin_addr.s_addr));

	//bind
	if (SOCKET_ERROR == ::bind(s, (struct sockaddr*)&sa, sizeof(sa)))
	{
		cout << "bad bind";
		system("pause");
	}
	
	//set socket to listen for connections
	if (SOCKET_ERROR == listen(s, SOMAXCONN))
	{
		cout << "bad listen";
		system("pause");
	}

	while (true)
	{
		char recvbuf[1024];
		unsigned int cnt{ 0 };
		stringstream o;

		//blocking accept call
		SOCKET client = accept(s, NULL, NULL);
		if(client != INVALID_SOCKET)
		{
			cout << endl << "got a client\n" << endl;

			//receive some data
			if ((cnt = recv(client, recvbuf, 1024, 0)) > 0)
			{
				o << string{ recvbuf,cnt };
			}

			cout << "received from client:\n" << o.str() << endl;

			//send back something
			string resp("ur good.\n\nhere's what client sent:\n");
			resp += o.str();
			send(client, resp.c_str(), resp.size(),0);
			break;
		}
		cout << ".";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	system("pause");
}

void X::clientside()
{
	//make socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "bad socket";
	}

	//setup address and port
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(5150);
	inet_pton(AF_INET, "127.0.0.1", &(sa.sin_addr.s_addr));

	//1Hz connect
	while (true)
		if (SOCKET_ERROR != connect(s, (struct sockaddr*)&sa, sizeof(sa)))
		{
			cout << endl << "found a server\n" << endl;

			stringstream ss;
			ss << string("GET /index.html HTTP/1.1\n");
			ss << string("Host: www.pants.com\n");
			ss << string("Accept: image/gif, image/jpeg, */*\n");
			ss << string("Accept-Language: en-us\n");
			ss << string("Accept-Encoding: gzip, deflate\n");
			ss << string("User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
			ss << string("\n");

			//send data
			send(s, ss.str().c_str(), (int)ss.str().size(), 0);

			//check for response
			char buf[1024];
			unsigned int x = recv(s, buf, 1024, 0);

			cout << "heard in response:\n" << string{ buf,x };
			break;
		}
		else
		{
			cout << ".";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
}

