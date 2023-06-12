#include "MetaTypeInclude.h"
namespace engine
{
void RegisterAllMetaClass()
{
    using namespace io;
    REGISTER_METATYPE(TcpServer);
}
}