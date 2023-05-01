#include "Headers.h"

TEST_CASE("PrintCommandTest", "PrintCommandTest")
{
    IPEndPoint endpoint("127.0.0.1", 1111);

    Socket client(IPv::IPv4);
    bool status = client.Create();
    REQUIRE(status == true);

    status = client.Connect(endpoint);

    REQUIRE(status == true);
}