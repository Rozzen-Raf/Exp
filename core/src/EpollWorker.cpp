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
int EpollWorker::worker_type = EPOLL;
//--------------------------------------------------------------------------------
EpollWorker::EpollWorker(WorkFlag flag) : WorkerBase(flag)
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

//    if(!std::holds_alternative<ID_t>(data->EventID))
//        return;

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

//    if(!std::holds_alternative<ID_t>(data->EventID))
//        return;

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
        AwaitableResult result;
       result.id = static_cast<ID_t>(ev.data.fd);

        if(ev.events & (EPOLLERR))
        {
            result.err = get_errno(ev.data.fd);
            result.type = Error;
            result.err_message = "Socket error";
        }
        else if(ev.events & (EPOLLHUP | EPOLLRDHUP))
        {
            result.err = 0;
            result.err_message = "Socket close";
            result.type = HangUp;
        }
        else if(ev.events & (EPOLLIN | EPOLLOUT))
        {
            result.err = 0;
            result.err_message = "";
            result.type = WakeUp;
        }

        switch(Flag)
        {
            case AllAwaiablesEmit:
                EmitWithAllFlags(std::move(result));
                break;
            case OnlyByID:
                EmitWithOnlyByIDFlags(std::move(result));
                break;
        }

    }
    co_return;
}
//--------------------------------------------------------------------------------

void EpollWorker::EmitWithAllFlags(AwaitableResult &&res)
{
    lock_t lock(mutex);

    if(Awaitables.empty())
        return;

    for(auto await = Awaitables.begin(); await != Awaitables.end(); ++await)
    {
        await->second->result = res;
        Emit(await->second, this);
    }
}
//--------------------------------------------------------------------------------

void EpollWorker::EmitWithOnlyByIDFlags(AwaitableResult &&res)
{
    std::unordered_map<UID_t, AwaitableData*>::iterator find;
    {
        lock_t lock(mutex);
        UID_t id = res.id;
        find = Awaitables.find(id);
        if(find == Awaitables.end())
            return;
    }

    find->second->result = std::move(res);

    Emit(find->second, this);
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
