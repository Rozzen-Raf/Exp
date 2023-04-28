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

int EpollWorker::Register(const std::any& fd_any)
{
    int fd = std::any_cast<int>(fd_any);
    epoll_event ev = {};
    ev.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET;
    ev.data.fd = fd;
    int ret = epoll_ctl(EpollFd, EPOLL_CTL_ADD, fd, &ev);
    return ret;
}
//--------------------------------------------------------------------------------

int EpollWorker::Unregister(const std::any& fd_any)
{
    int fd = std::any_cast<int>(fd_any);
    int ret = epoll_ctl(EpollFd, EPOLL_CTL_DEL, fd, nullptr);
    return ret;
}
//--------------------------------------------------------------------------------
