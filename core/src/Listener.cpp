#include "Listener.h"

CoroTask<AwaitableResult> Listener::AsyncAccept(ShedulerSharedPtr sheduler, WorkerType type) noexcept
{
	int ret;
	std::cout << "async_accept start" << std::endl;
	while((ret = accept4(fd_, nullptr, nullptr, SOCK_NONBLOCK)) == -1)
	{
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            co_return AwaitableResult{Error, fd_, "Failed to accept", errno};

		auto status = co_await sheduler->event(type, fd_);
		if(!status)
            co_return status;
	}
    Register->Register(ret);
    co_return AwaitableResult{WakeUp, ret, "", 0};
}
//--------------------------------------------------------------------------------------
AwaitableResult Listener::Accept() noexcept
{
    int ret;
    std::cout << "async_accept start" << std::endl;
    while((ret = accept(fd_, nullptr, nullptr)) == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            return {Error, fd_, "Failed to accept", errno};
        continue;
    }
    Register->Register(ret);
    return {Success, ret, "", 0};
}
//--------------------------------------------------------------------------------------

