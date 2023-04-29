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

CoroTask<AwaitableResult> Socket::async_read(ShedulerSharedPtr sheduler, buffer& read_bf)
{
   // auto shifting_data = read_bf;
    while(true)
    {
        int64_t cnt = recv(fd_, read_bf.data(), read_bf.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

        if(cnt == -1 && errno != EAGAIN)
        {
            ERROR(Socket, "Failes too read data from socket");
            co_return {Error};
        }

        if(cnt == -1 && errno == EAGAIN)
        {
            auto status = co_await sheduler->event(EPOLL, fd_);
            if(status.type != WakeUp)
                co_return status;
            else
                continue;
            continue;
        }

        read_bf.resize(cnt);
        break;
    }

    co_return {WakeUp};
}
//----------------------------------------------------------

CoroTask<AwaitableResult> Socket::async_write(ShedulerSharedPtr sheduler, buffer write_bf)
{
    size_t cnt = send(fd_, write_bf.data() , write_bf.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

    if(cnt == -1 && errno != EAGAIN)
    {
        ERROR(Socket, "Failes too write data from socket");
        co_return {Error};
    }

    auto status = co_await sheduler->event(EPOLL, fd_);
    if(status.type != WakeUp)
            co_return status;

    co_return {WakeUp};
}
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
