#include "Listener.h"

CoroTask<AwaitableResult> Listener::AsyncAccept(ShedulerSharedPtr sheduler) noexcept
{
	int ret;
	std::cout << "async_accept start" << std::endl;
	while((ret = accept4(fd_, nullptr, nullptr, SOCK_NONBLOCK)) == -1)
	{
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            co_return {Error, fd_, "Failed to accept", errno};

		auto status = co_await sheduler->event(EPOLL, fd_);
		if(!status)
            co_return status;
	}
    Register->Register(ret);
    co_return {WakeUp, ret, "", 0};
}
