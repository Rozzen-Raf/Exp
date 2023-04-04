#pragma once
#include <coroutine>
#include <utility>
#include <exception>
#include <functional>
#include <memory>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <variant>
#include <map>
#include <future>
#include <stdint.h>
#include <type_traits>
#include "UID.h"

typedef std::function<void()> func_t;
typedef std::string String;
typedef int ID_t;
typedef std::variant<String, ID_t> UID_t;

enum ControlType
{

};

enum EventType
{

};

struct AwaitableResult
{
	EventType type;
};

struct AwaitableData
{
	UID_t EventID;
	std::coroutine_handle<> continuation;
	AwaitableResult result;
};
