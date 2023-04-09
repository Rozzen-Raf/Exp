#pragma once
#include "Types.h"

template<typename T>
concept TaskWithFunc = requires(T x)
{
    {x->GetFunc()};
};
