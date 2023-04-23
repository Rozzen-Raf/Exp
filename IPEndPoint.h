#ifndef IPEndPoint_H
#define IPEndPoint_H

#include "Types.h"

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
private:
    IPv Version = IPv4;
    uint16_t Port = 0;
    std::string IP;
    std::vector<uint8_t> IPBytes;
};

#endif
