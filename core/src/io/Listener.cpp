#include "Listener.h"

namespace io{

task::CoroTask<engine::AwaitableResult> Listener::AsyncAccept(task::ShedulerSharedPtr sheduler, engine::WorkerType type) noexcept
{
	int ret;
	std::cout << "async_accept start" << std::endl;
	while((ret = accept4(fd_, nullptr, nullptr, SOCK_NONBLOCK)) == -1)
	{
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            co_return engine::AwaitableResult{engine::Error, fd_, "Failed to accept", errno};

		auto status = co_await sheduler->event(type, fd_);
		if(!status)
            co_return status;
	}
    Register->Register(ret);
    co_return engine::AwaitableResult{engine::WakeUp, ret, "", 0};
}
//--------------------------------------------------------------------------------------
engine::AwaitableResult Listener::Accept() noexcept
{
    int ret;
    std::cout << "async_accept start" << std::endl;
    while((ret = accept(fd_, nullptr, nullptr)) == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            return engine::AwaitableResult{engine::Error, fd_, "Failed to accept", errno};
        continue;
    }
    Register->Register(ret);
    return engine::AwaitableResult{engine::Success, ret, "", 0};
}
//--------------------------------------------------------------------------------------
}

