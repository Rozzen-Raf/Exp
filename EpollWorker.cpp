#include "EpollWorker.h"
EpollWorker::EpollWorker()
	{
        if (EpollFd != -1)
            return;
        EpollFd = epoll_create(1);
        if (EpollFd == -1)
            throw std::system_error(errno, std::system_category(),
                "Failed to create epoll socket.");
	}
//--------------------------------------------------------------------------------

void EpollWorker::RegAwaitable(AwaitableData* data) noexcept
	{
		if (data->type != GetType())
			return;

		if(!std::holds_alternative<ID_t>(data->EventID))
			return;

		ID_t fd = std::get<ID_t>(data->EventID);
		lock_t lock(mutex);

		Awaitables.emplace(data->EventID, data );
	}
//--------------------------------------------------------------------------------

void EpollWorker::UnregAwaitable(AwaitableData* data) noexcept
	{
		if(data->type != GetType())
		{
			return;
		}

		if(!std::holds_alternative<ID_t>(data->EventID))
			return;

		ID_t fd = std::get<ID_t>(data->EventID);

		lock_t lock(mutex);

		Awaitables.erase(data->EventID);
	}
//--------------------------------------------------------------------------------

CoroTaskVoid EpollWorker::Run()
	{
        epoll_event ev;
		stop = false;
		while (!stop)
		{
            if(Awaitables.empty())
            {
                std::cout << "wait" << std::endl;
                co_await std::suspend_always{};
            }
			int ret = epoll_wait(EpollFd, &ev, 1, 5000);

            std::cout << ret << "!" << std::endl;

            if(ret == -1)
            {
                throw std::system_error(errno, std::system_category(),
                              "Failed to fetch epoll event.");
            }

            if(ret == 0)
                continue;
			std::unordered_map<UID_t, AwaitableData*>::iterator find;
            {
                lock_t lock(mutex);
                UID_t id = static_cast<ID_t>(ev.data.fd);
                find = Awaitables.find(id);
                if(find == Awaitables.end())
                    continue;
            }

            if(ev.events & (EPOLLIN | EPOLLOUT))
            {
                find->second->result.type = WakeUp;
            }

            Emit(find->second, this);
		}
        co_return;
	}
//--------------------------------------------------------------------------------

void EpollWorker::Stop() noexcept
{
	stop = true;
}
//--------------------------------------------------------------------------------

int EpollRegister(int fd, int epoll_fd)
{
    epoll_event ev = {};
  ev.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET;
  ev.data.fd = fd;
  int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
  return ret;
}
//--------------------------------------------------------------------------------

int EpollDeregister(int fd, int epoll_fd)
{
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    return ret;
}
//--------------------------------------------------------------------------------