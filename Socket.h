#ifndef SSSocket_H
#define SSSocket_H

#include "ChainableTask.h"
#include "Types.h"
#include "IPEndPoint.h"
#include "RegisterMediator.h"
class Socket
{
public:
	static constexpr uint64_t BUFFER_SIZE = 1024;

    Socket()
    {
        Create();
    }

    explicit Socket(RegisterMediatorBasePtr mediator) : RegMediator(mediator)
	{
        Create();
	}

    explicit Socket(IPv version, int fd = -1, RegisterMediatorBasePtr mediator = nullptr) : fd_(fd), Version(version), RegMediator(mediator)
	{

	}

    ~Socket()
    {
        Close();
    }

//	SSocket(const SSocket& sock) : fd_(sock.fd_), Version(sock.Version){}

    Socket(Socket&& sock) : fd_(std::move(sock.fd_)), Version(std::move(sock.Version))
    {
        sock.fd_ = -1;
    }

//	SSocket& operator=(const SSocket& fd)
//	{
//		fd_ = fd.fd_;
//		Version = fd.Version;
//		return *this;
//	}

    Socket& operator=(Socket&& fd)
	{
		fd_ = std::move(fd.fd_);
        fd.fd_ = -1;;
		Version = std::move(fd.Version);
		return *this;
	}

	inline void Close()
	{
        if(isValid())
        {
            shutdown(fd_, 2);
            close(fd_);
        }
	}

    inline int Desc() const noexcept { return fd_; }

	bool Connect(const IPEndPoint& end_point);
	bool Create();
	bool Bind(const IPEndPoint& endpoint);
	bool Listen(const IPEndPoint& endpoint);
    Socket Accept();

	// PSBuffer read();
	// int write(PSBuffer buffer);

	// chainable_task<PSBuffer> async_read(TaskProcessorContext* task_processor);
	// chainable_task_void async_read(TaskProcessorContext* task_processor, const callback_f& callback);

	// chainable_task<int> async_write(TaskProcessorContext* task_processor, PSBuffer buffer);

	bool isValid() const noexcept
	{
		return fd_ >= 0;
	}
private:
    void RegisterMediator();
private:
	int fd_ = -1;
	IPv Version;
	RegisterMediatorBasePtr RegMediator;
};
DECLARE_SHARED_PTR(Socket)

#endif // ! SSSocket_H
