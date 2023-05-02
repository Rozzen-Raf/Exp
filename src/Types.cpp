#include "Types.h"

buffer_ptr ConvertStringToBuffer(const String& string)
{
    buffer_ptr bytes = std::make_shared<buffer>();

    bytes->reserve(string.size());

    std::transform(std::begin(string), std::end(string), std::back_inserter(*bytes.get()), [](char c){
        return std::byte(c);
    });

    return bytes;
}