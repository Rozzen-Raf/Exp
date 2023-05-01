#include "EpollWorker.h"
//--------------------------------------------------------------------------------
int get_errno(int fd) {
  int error = 0;
  socklen_t errlen = sizeof(error);
  if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)&error, &errlen) == 0) {
    return error;
  } else {
    return errno;
  }
}
//--------------------------------------------------------------------------------
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
		int ret = epoll_wait(EpollFd, &ev, 1, 5000);

        if(ret == -1)
        {
            continue;
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

        find->second->result.id = static_cast<ID_t>(ev.data.fd);

        if(ev.events & (EPOLLERR))
        {
            find->second->result.err = get_errno(ev.data.fd);
            find->second->result.type = Error;
            find->second->result.err_message = "Socket error";
        }
        else if(ev.events & (EPOLLHUP | EPOLLRDHUP))
        {
            find->second->result.err = 0;
            find->second->result.err_message = "Socket close";
            find->second->result.type = HangUp;
        }
        else if(ev.events & (EPOLLIN | EPOLLOUT))
        {
            find->second->result.err = 0;
            find->second->result.err_message = "";
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
