#include "Headers.h"
#include "HttpParser.h"
#include "JsonParser.h"

TEST_CASE("PrintCommandTest", "PrintCommandTest")
{
    io::IPEndPoint endpoint("127.0.0.1", 1111);

    io::Socket client(io::IPv::IPv4);
    bool status = client.Create();
    REQUIRE(status == true);

    status = client.Connect(endpoint);

    REQUIRE(status == true);

    String message = "POST /print HTTP/1.1\r\n"
                    "User-Agent: Mozilla/5.0\r\n"
                    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                    "Host: 127.0.0.1\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: 70\r\n"
                    "\r\n{\"Command\" : \"Print\", \"MessageID\" : 1, \"PrintData\" : \"Hello, world!\"}";

    auto write_bf = std::as_bytes(std::span(message));
    GetSheduler()->CoroStart(client.async_write(GetSheduler().get(), write_bf, engine::EPOLL));

    std::byte buf[256];
    buffer_view read_buffer = std::as_writable_bytes(std::span(buf));
    auto result = client.read(read_buffer);

    REQUIRE(result);

    parse::JsonParser parser;
    status = parser.Parse(read_buffer);

    REQUIRE(status == true);

    auto message_id_opt = parser.GetValue<ID_t>("MessageID");

    REQUIRE(message_id_opt.has_value());

    REQUIRE(message_id_opt.value() == 1);

    auto status_opt = parser.GetValue<ID_t>("ResultID");

    REQUIRE(status_opt.has_value());

    REQUIRE(status_opt.value() == static_cast<ID_t>(api::Result::Success));
}
