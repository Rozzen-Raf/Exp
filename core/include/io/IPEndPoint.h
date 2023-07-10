#ifndef IPEndPoint_H
#define IPEndPoint_H

#include "Types.h"

namespace io
{
enum IPv
{
    Unknown,
    IPv4,
    IPv6
};

class IPEndPoint
{
public:
    IPEndPoint() = default;
    IPEndPoint(const char* ip, uint16_t port);
    explicit IPEndPoint(sockaddr* addr);

    sockaddr_in GetSockaddrIPv4() const noexcept;
    const String& GetIP() const noexcept;
    uint16_t GetPort() const noexcept;
    IPv GetVersion() const noexcept;
private:
    IPv Version = IPv4;
    uint16_t Port = 0;
    String IP;
    std::vector<uint8_t> IPBytes;
};
}
#endif
