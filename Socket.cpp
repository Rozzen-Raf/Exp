#include "Socket.h"

bool Socket::Connect(const IPEndPoint& end_point)
{
    sockaddr_in addr = end_point.GetSockaddrIPv4();
    if(connect(fd_, (sockaddr*)& addr, sizeof(addr)) != 0)
    {
        ERROR(SSocket, std::string("Socket connect error: ") + strerror(errno));
        Close();
        return false;
    }

    return true;
}
//-----------------------------------------------

bool Socket::Create()
{
	fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(fd_ < 0)
	{
		ERROR(SSocket, std::string("Socket not created: ") + strerror(errno));
		return false;
	}
	
	return true;
}
//----------------------------------------------------------

bool Socket::Bind(const IPEndPoint& endpoint)
{
    sockaddr_in addr = endpoint.GetSockaddrIPv4();
	int result = bind(fd_, (sockaddr*)(&addr), sizeof(sockaddr_in));
    if (result != 0)
    {
		ERROR(SSocket, std::string("Bind error: ") + strerror(errno));
		return false;
	}
	return true;
}
//----------------------------------------------------------

bool Socket::Listen(const IPEndPoint& endpoint)
{
	if(!Bind(endpoint))
	{
		return false;
	}

	int result = listen(fd_, SOMAXCONN);

	if(result != 0)
	{
		ERROR(SSocket, std::string("Listen error: ") + strerror(errno));
		return false;
	}

	return true;
}
//----------------------------------------------------------

CoroTaskVoid Socket::async_read(ShedulerSharedPtr sheduler, buffer& read_bf)
{
	co_return;
}
//----------------------------------------------------------

CoroTaskVoid Socket::async_write(ShedulerSharedPtr sheduler, const buffer& write_bf)
{
	co_return;
}
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
