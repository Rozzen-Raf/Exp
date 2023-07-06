#include "Headers.h"
#include "JsonParser.h"

TEST_CASE("ParseData", "ParseData")
{
    String message = "{\
        \"Command\" : \"Print\",\
        \"MessageID\" : 1,\
        \"PrintData\" : \"Hello, world!\"\
	}";
    auto data_opt = parse::JsonParser::ParseToData(std::as_bytes(std::span(message)));

    REQUIRE(data_opt.has_value());

    parse::ParseData data = data_opt.value();

    auto command_opt = data.GetValue<String>("Command");
    REQUIRE(command_opt.has_value());

    REQUIRE(command_opt.value() == "Print");
}