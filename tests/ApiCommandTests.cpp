#include "Headers.h"

TEST_CASE("PrintCommandTest", "PrintCommandTest")
{
    IPEndPoint endpoint("127.0.0.1", 11111);

    Socket client(IPv::IPv4);
    bool status = client.Create();
    REQUIRE(status == true);

    status = client.Connect(endpoint);

    REQUIRE(status == true);

    String message = "{\
        \"ApiCommand\" : {\
            \"Command\" : \"Print\",\
            \"MessageID\" : 1,\
            \"PrintData\" : \"Hello, world!\"\
        }\
	}";

    auto write_bf = std::as_bytes(std::span(message));
    GetSheduler()->CoroStart(client.async_write(GetSheduler(), write_bf));

    buffer read_bf = buffer(256);
    auto result = client.read(read_bf);

    REQUIRE(result);

    JsonParser parser;
    status = parser.Parse(read_bf);

    REQUIRE(status == true);

    auto message_id_opt = parser.GetValue<ID_t>("MessageID");

    REQUIRE(message_id_opt.has_value());

    REQUIRE(message_id_opt.value() == 1);

    auto status_opt = parser.GetValue<ID_t>("ResultID");

    REQUIRE(status_opt.has_value());

    REQUIRE(status_opt.value() == static_cast<ID_t>(Result::Success));
}
