#pragma once
#include "Types.h"
class IParser
{
public:
    virtual std::optional<std::any> GetValue(const String& key) const = 0;
    virtual void SetValue(const String& key, const std::any& value) = 0;
    virtual String Dump() const = 0;
};