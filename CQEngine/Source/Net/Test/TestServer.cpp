#if defined(_MSC_VER)
	#define WIN32_LEAN_AND_MEAN
	#include<windows.h>
	#include<WinSock2.h>

	#pragma comment(lib,"ws2_32.lib")
#else
	#include<unistd.h> 
	#include<arpa/inet.h>
	#include<string.h>

	#define SOCKET int
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
#endif

#include <stdio.h>
#include <vector>
#include "proto.h"

int work(SOCKET _cSock)
{
	Header head = {};
	int len = recv(_cSock, (char*)&head, sizeof(Header), 0);
	if (len > 0)
	{
		printf("client package size = %d.\n", head.len_);
	}
	else if (len == 0)
	{
		puts("client exit.");
		return 0;
	}
	else
	{
		puts("recv msg exception.");
		return -1;
	}

	switch (head.code_)
	{
	case LOGIN:
	{
		LoginPackage login = {};
		int ret = recv(_cSock, (char*)&login + sizeof(Header), head.len_ - sizeof(Header), 0);
		if (len > 0)
		{
			printf("client login name = %s,pwd = %s.\n", login.name_, login.pwd_);
		}

		LoginRetPackage retPak = {};
		strcpy(retPak.result_, "token is right!");
		send(_cSock, (char*)&retPak, sizeof(retPak), 0);
	}
		return 1;
	default:
		puts("ERROR : CLIENT DATA INVAILD.");
		return 0;
	}

	return 1;
}

#if 0
int main(int argc, char *argv[])
{
#if defined(_MSC_VER)
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif
	SOCKET s_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s_sock == INVALID_SOCKET)
	{
		puts("CREATE SOCKET FAIL.");
		return -1;
	}
	puts("INIT SOCKET SUCCESS.");

	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
#if defined(_MSC_VER)
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
	sin.sin_addr.s_addr = INADDR_ANY;
#endif
	int ret = bind(s_sock, (sockaddr*)&sin, sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		printf("BIND SOCKET FAIL, RET_CODE = %d.\n", ret);
		return -1;
	}
	puts("SERVER BIND SUCCESS.");

	ret = listen(s_sock, 100);
	if (ret == SOCKET_ERROR)
	{
		printf("LISTEN SOCKET FAIL, RET_CODE = %d.\n", ret);
		return -1;
	}
	puts("SERVER LISTEN SUCCESS.");
	puts("===== SERVER INIT SUCCESS ======");

	std::vector<SOCKET> c_socks;
	do
	{
		// init fd_set
		fd_set r_fd_set;
		fd_set w_fd_set;
		fd_set exp_fd_set;

		FD_ZERO(&r_fd_set);
		FD_ZERO(&w_fd_set);
		FD_ZERO(&exp_fd_set);

		// set fd_Set
		FD_SET(s_sock, &r_fd_set);
		FD_SET(s_sock, &w_fd_set);
		FD_SET(s_sock, &exp_fd_set);

		SOCKET max_sock = s_sock;
		for (int i = 0 ; i < c_socks.size();++i)
		{
			FD_SET(c_socks[i], &r_fd_set);
			max_sock = max_sock < c_socks[i] ? c_socks[i] : max_sock;
		}

		// select
		timeval tv = {0,0};
		int ret = select(max_sock + 1,&r_fd_set,&w_fd_set,&exp_fd_set,&tv);
		if (ret < 0)
		{
			puts("SERVER SELECT ERROR.");
			break;
		}
		// handle server sock : new client accept
		if (FD_ISSET(s_sock, &r_fd_set))
		{
			FD_CLR(s_sock, &r_fd_set);

			sockaddr_in cAddr = {};
			int cAddrLen = sizeof(sockaddr_in);
			SOCKET sock = INVALID_SOCKET;
#if defined(_MSC_VER)
			sock = accept(s_sock, (sockaddr*)&cAddr, &cAddrLen);
#else
			sock = accept(s_sock, (sockaddr*)&cAddr, (socklen_t *)&cAddrLen);
#endif
			if (sock == INVALID_SOCKET)
			{
				puts("SERVER ACCEPT CLIENT ERROR.");
			}
			else
			{
				c_socks.push_back(sock);
				printf("SERVER ACCEPT NEW CLIENT FROM :%s\n", inet_ntoa(cAddr.sin_addr));
			}
		}

		// handle clients sock : recv message
		for (int i = 0; i < c_socks.size(); ++i)
		{
			if (FD_ISSET(c_socks[i], &r_fd_set))
			{
				int ret = work(c_socks[i]);
				if (ret <= 0)
				{
#if defined(_MSC_VER)
					closesocket(c_socks[i]);
#else
					close(c_socks[i]);
#endif
					c_socks.erase(c_socks.begin() + i);
					puts("close client socket.");
				}
			}
		}
		
	} while (true);

	// clean
#if defined(_MSC_VER)
	for (int i = 0; i < c_socks.size(); ++i)
	{
		closesocket(c_socks[i]);
	}
	closesocket(s_sock);
	WSACleanup();
#else
	for (int i = 0; i < c_socks.size(); ++i)
	{
		close(c_socks[i]);
	}
	close(s_sock);
#endif
	puts("Bay.");
	return 0;
}
#endif 