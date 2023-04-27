#include "Listener.h"

CoroTask<Socket> Listener::AsyncAccept(ShedulerWeakPtr sheduler) noexcept
{
	int ret;
	std::cout << "async_accept start" << std::endl;
	while((ret = accept4(fd_, nullptr, nullptr, SOCK_NONBLOCK)) == -1)
	{
		auto status = co_await sheduler->event(EPOLL, fd_);
		if(status.type != WakeUp)
			co_return {};
	}
	std::cout << ret << std::endl;
	shutdown(ret,2);
	close(ret);
}