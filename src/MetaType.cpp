#include "MetaType.h"
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

