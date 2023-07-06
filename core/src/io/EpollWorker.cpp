#include "EpollWorker.h"
//--------------------------------------------------------------------------------
namespace io
{
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
EpollWorker::EpollWorker() : engine::WorkerBase()
{
    if (EpollFd != -1)
        return;
    EpollFd = epoll_create(1);
    if (EpollFd == -1)
        throw std::system_error(errno, std::system_category(),
            "Failed to create epoll socket.");
}
//--------------------------------------------------------------------------------

void EpollWorker::RegAwaitable(engine::AwaitableData* data) noexcept
{
    if (data->type != GetType())
        return;

    lock_t lock(mutex);

    Awaitables.emplace(data->EventID, data );
}
//--------------------------------------------------------------------------------

void EpollWorker::UnregAwaitable(engine::AwaitableData* data) noexcept
{
    if(data->type != GetType())
    {
        return;
    }

    lock_t lock(mutex);

    Awaitables.erase(data->EventID);
}
//--------------------------------------------------------------------------------

task::CoroTaskVoid EpollWorker::Run()
{
    epoll_event ev;
	stop = false;
	while (!stop)
	{
		int ret = epoll_wait(EpollFd, &ev, 1, 5000);

        if(ret == -1 || ret == 0)
        {
            continue;
        }

       engine::AwaitableResult result;
       result.id = static_cast<ID_t>(ev.data.fd);

        if(ev.events & (EPOLLERR))
        {
            result.err = get_errno(ev.data.fd);
            result.type = engine::Error;
            result.err_message = "Socket error";
        }
        else if(ev.events & (EPOLLHUP | EPOLLRDHUP))
        {
            result.err = 0;
            result.err_message = "Socket close";
            result.type = engine::HangUp;
        }
        else if(ev.events & (EPOLLIN | EPOLLOUT))
        {
            result.err = 0;
            result.err_message = "";
            result.type = engine::WakeUp;
        }

        std::unordered_map<UID_t, engine::AwaitableData*>::iterator find;
        {
            lock_t lock(mutex);
            UID_t id = result.id;
            find = Awaitables.find(id);
            if(find == Awaitables.end())
                continue;
        }

        find->second->result = std::move(result);
        find->second->Worker = this;

        Emit(find->second);
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
}