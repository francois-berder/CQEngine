#include "CQSocket.h"
#include "CQDebug.h"

USING_NS_CQ

CQSocket::CQSocket()
	: 
	socket_(INVALID_SOCKET),
	desc_("CQSOCKET")
{
	if (!Init())
	{
		Clean();
	}
}

CQSocket::CQSocket(PROTO_TYPE _pType, IP_TYPE _ipType)
	: 
	socket_(INVALID_SOCKET),
	desc_("CQSOCKET")
{
	if (!Init(_pType, _ipType))
	{
		Clean();
	}
}

CQSocket::~CQSocket() 
{
	Clean();
}

bool CQSocket::Init()
{
	return Init(CQEngine::CQSocket::TCP, CQEngine::CQSocket::IPV4);
}

bool CQSocket::Init(PROTO_TYPE _pType, IP_TYPE _ipType)
{
	if (IsValid())
	{
		puts("[CQSOCKET] SOCKET INIT ALREADY.");
		return true;
	}

	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_pType == CQEngine::CQSocket::UDP)
	{
		//socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	}
	if (_ipType == CQEngine::CQSocket::IPV6)
	{
		//socket_ = socket(AF_INET6, SOCK_STREAM, IPPROTO_UDP);
	}

	if (socket_ == INVALID_SOCKET)
	{
		printf("[CQSocket] SOCKET INIT ERROR :%d\n", getSocketError());
		return false;
	}
	puts("[CQSOCKET] SOCKET INIT SUCCESS.");
	return true;
}

void CQSocket::Clean()
{
	if (!IsValid())
	{
		puts("[CQSOCKET] SOCKET CLEAN ALREADY.");
		return;
	}

#if defined(_MSC_VER)
	closesocket(socket_);
#else
	close(socket_);
#endif

	socket_ = INVALID_SOCKET;
	puts("[CQSOCKET] SOCKET CLEAN SUCCESS.");
}

bool CQSocket::Connect(const std::string _ip, const short _port)
{
	if (!IsValid())
	{
		puts("[CQSOCKET] SOCKET CONNECT FAIL.");
		return false;
	}

	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_port);
#if defined(_MSC_VER)
	sin.sin_addr.S_un.S_addr = inet_addr(_ip.c_str());
#else
	sin.sin_addr.s_addr = inet_addr(_ip.c_str());
#endif
	int ret = connect(socket_, (sockaddr*)&sin, sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		printf("[CQSocket] SOCKET CONNECT ERROR :%d\n", getSocketError());
		return false;
	}
	puts("[CQSOCKET] SOCKET CONNECT SUCCESS.");
	return true;
}

int CQSocket::Send(const char *_buf,const int _bufLen,int _sig /*= 0*/)
{
	int ret = send(socket_, _buf, _bufLen, _sig);
	return ret;
}

int CQSocket::Recv(char *_buf, int _bufLen, int _sig /*= 0*/)
{
	int ret = recv(socket_, _buf, _bufLen, _sig);
	return ret;
}

bool CQSocket::IsReadAble()
{
	if (!IsValid())
	{
		return false;
	}

	fd_set r_fd_set;
	FD_ZERO(&r_fd_set);
	FD_SET(socket_, &r_fd_set);

	// select
	timeval tv = { 0,0 };
	int ret = select(socket_ + 1, &r_fd_set, 0, 0, &tv);
	
	if (ret > 0)
	{
		if (FD_ISSET(socket_, &r_fd_set))
		{
			FD_CLR(socket_, &r_fd_set);
			return true;
		}
	}
	// ret == 0 : select time out
	// ret == -1
	if (ret == -1)
	{
		printf("[CQSocket] SOCKET ISREADABLE ERROR :%d\n", getSocketError());
	}
	return false;
}

bool CQSocket::IsWriteAble()
{
	if (!IsValid())
	{
		return false;
	}

	fd_set w_fd_set;
	FD_ZERO(&w_fd_set);
	FD_SET(socket_, &w_fd_set);

	// select
	timeval tv = { 0,0 };
	int ret = select(socket_ + 1, 0, &w_fd_set, 0, &tv);

	if (ret > 0)
	{
		if (FD_ISSET(socket_, &w_fd_set))
		{
			FD_CLR(socket_, &w_fd_set);
			return true;
		}
	}

	// ret == 0 : select time out
	// ret == -1
	if (ret == -1)
	{
		printf("[CQSocket] SOCKET ISWRITEABEL ERROR :%d\n", getSocketError());
	}
	return false;
}

bool CQSocket::IsValid()
{
	return socket_ == INVALID_SOCKET ? false : true;
}