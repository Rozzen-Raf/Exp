#ifndef SSSocket_H
#define SSSocket_H

#include "ChainableTask.h"
#include "Types.h"
#include "IPEndPoint.h"
#include "Sheduler.h"
class Socket
{
public:
	static constexpr uint64_t MSG_SIZE = 1024;

    Socket() noexcept
    {
        Create();
    }

    explicit Socket(IPv version, int fd = -1) noexcept : fd_(fd), Version(version)
	{

	}

    ~Socket()
    {
        Close();
    }

    Socket(Socket&& sock) noexcept : fd_(std::move(sock.fd_)), Version(std::move(sock.Version))
    {
        sock.fd_ = -1;
    }

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

    CoroTask<AwaitableResult> async_read(ShedulerSharedPtr sheduler, buffer_view& read_bf);
    CoroTask<AwaitableResult> async_write(ShedulerSharedPtr sheduler, buffer_view_const& write_bf);

    AwaitableResult read(buffer_view& read_bf);
    AwaitableResult write(buffer_view_const& write_bf);

	bool isValid() const noexcept
	{
		return fd_ >= 0;
	}

protected:
	int fd_ = -1;
	IPv Version;
};
DECLARE_SHARED_PTR(Socket)

#endif // ! SSSocket_H
