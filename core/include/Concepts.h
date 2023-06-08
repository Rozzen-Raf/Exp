#pragma once
#include "Types.h"

template<typename T>
concept CoroutineHandle = std::convertible_to<T, std::coroutine_handle<>>;

template<typename T>
concept HandleWithCallback = CoroutineHandle<T> && requires(T h)
{
    {h.promise().done_callback};
};
