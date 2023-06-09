#include "MetaTypeInclude.h"
METATYPE_DEF_TEMPL(Print<JsonParser>)
void RegisterAllMetaClass()
{
    REGISTER_METATYPE(TcpServer);
    REGISTER_METATYPE_TEMPL(Print, JsonParser);
}