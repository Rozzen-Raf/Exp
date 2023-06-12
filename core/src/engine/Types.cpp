#include "Types.h"
namespace engine
{
buffer_ptr ConvertStringToBuffer(const String& string)
{
    buffer_ptr bytes = std::make_shared<buffer>();

    bytes->reserve(string.size());

    std::transform(std::begin(string), std::end(string), std::back_inserter(*bytes.get()), [](char c){
        return std::byte(c);
    });

    return bytes;
}

ID_t GetLocalTime()
{
   return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
}
