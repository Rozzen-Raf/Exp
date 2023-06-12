#include "IPEndPoint.h"

namespace io
{
IPEndPoint::IPEndPoint(const char* ip, uint16_t port)
{
    Port = port;
    in_addr addr;

    int result = inet_pton(AF_INET, ip, &addr);

	if (result == 1)
	{
		if (addr.s_addr != INADDR_NONE)
		{
			IP = ip;
			Version = IPv::IPv4;
		}
	}      
}

IPEndPoint::IPEndPoint(sockaddr* addr)
{
	sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);	
    if(addrv4)
    {	
        Version = IPv::IPv4;
	    Port = ntohs(addrv4->sin_port);
	    IP.resize(16);
	    inet_ntop(AF_INET, &addrv4->sin_addr, &IP[0], 16);
    }
}

sockaddr_in IPEndPoint::GetSockaddrIPv4() const noexcept
{
    sockaddr_in addr;

    addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP.c_str());
	addr.sin_port = htons(Port);

	return addr;
}
}