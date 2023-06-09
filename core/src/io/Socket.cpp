#include "Socket.h"
namespace io
{
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

task::CoroTask<engine::AwaitableResult> Socket::async_read(task::Sheduler* sheduler, buffer_view& read_bf, engine::WorkerType type)
{
    if(read_bf.empty())
        co_return engine::AwaitableResult{engine::Error, fd_, "Read buffer is empty", -1};

    while(true)
    {
        ssize_t cnt = recv(fd_, read_bf.data(), read_bf.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

        if(cnt == -1 && errno != EAGAIN)
        {
            co_return engine::AwaitableResult{engine::Error, fd_, "Failes too read data from socket", errno};
        }

        if(cnt == 0)
            break;

        if(cnt == -1 && errno == EAGAIN)
        {
            auto status = co_await sheduler->event(type, fd_);
            if(!status)
                co_return status;
            continue;
        }

        break;
    }

    co_return engine::AwaitableResult{engine::WakeUp};
}
//----------------------------------------------------------

task::CoroTask<engine::AwaitableResult> Socket::async_write(task::Sheduler* sheduler, buffer_view_const& write_bf, engine::WorkerType type)
{
    if(write_bf.empty())
        co_return engine::AwaitableResult{engine::Error, fd_, "Write buffer is empty", -1};

    ssize_t written_bytes = 0;
    while(written_bytes < write_bf.size())
    {
        ssize_t cnt = send(fd_, write_bf.data(), write_bf.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

        if(cnt == -1 && errno != EAGAIN)
        {
            co_return engine::AwaitableResult{engine::Error, fd_, "Failes too write data from socket", errno};
        }
        else if(cnt == -1 && errno == EAGAIN)
        {
            auto status = co_await sheduler->event(type, fd_);
            if(!status)
                co_return status;

            continue;
        }
        else if(cnt > 0)
        {
            written_bytes += cnt;
        }
    }

    co_return engine::AwaitableResult{engine::WakeUp};
}
//----------------------------------------------------------

engine::AwaitableResult Socket::read(buffer_view& read_bf)
{
    if(read_bf.empty())
        return engine::AwaitableResult{engine::Error, fd_, "Read buffer is empty", -1};

    while(true)
    {
        int cnt = recv(fd_, read_bf.data(), read_bf.size(), MSG_NOSIGNAL);

        if(cnt == -1 && errno != EAGAIN)
        {
            return engine::AwaitableResult{engine::Error, fd_, "Failes too read data from socket", errno};
        }
        else if(cnt == -1 && errno == EAGAIN)
        {
            continue;
        }

        break;
    }

    return engine::AwaitableResult{engine::Success};
}
//----------------------------------------------------------

engine::AwaitableResult Socket::write(buffer_view_const& write_bf)
{
    if(write_bf.empty())
        return engine::AwaitableResult{engine::Error, fd_, "Write buffer is empty", -1};

    ssize_t written_bytes = 0;
    while(written_bytes < write_bf.size())
    {
        ssize_t cnt = send(fd_, write_bf.data(), write_bf.size(), MSG_NOSIGNAL);

        if(cnt == -1 && errno != EAGAIN)
        {
            return engine::AwaitableResult{engine::Error, fd_, "Failes too write data from socket", errno};
        }
        else if(cnt == -1 && errno == EAGAIN)
        {
            continue;
        }
        else if(cnt > 0)
        {
            written_bytes += cnt;
        }
    }

    return engine::AwaitableResult{engine::Success};
}
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
}
