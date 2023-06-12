#include "MetaType.h"
namespace engine
{
//-----------------------------------------------------------------
MetaDataUniquePtr MetaData::StaticMetaData = nullptr;
//-----------------------------------------------------------------

MetaData* MetaData::GetMetaData()
{
    if(!StaticMetaData)
        StaticMetaData = std::make_unique<MetaData>();

    return StaticMetaData.get();
}
//-----------------------------------------------------------------
}

