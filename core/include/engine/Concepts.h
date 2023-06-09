#pragma once
#include "Types.h"
namespace engine{
template<typename T>
concept CoroutineHandle = std::convertible_to<T, std::coroutine_handle<>>;

template<typename T>
concept HandleWithCallback = CoroutineHandle<T> && requires(T h)
{
    {h.promise().done_callback};
};

template<typename T, typename... Arg>
concept HasRegister = requires(T t, Arg... a)
{
    {t.Register(a...)};
};

template<typename T, typename... Arg>
concept HasUnregister = requires(T t, Arg... a)
{
    {t.Unregister(a...)};
};
}
