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
#include <unordered_map>
#include <future>
#include <stdint.h>
#include <type_traits>
#include <atomic>
#include "UID.h"

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#endif

typedef std::function<void()> func_t;
typedef std::string String;
typedef int ID_t;
typedef std::variant<String, ID_t> UID_t;

enum WorkerType
{
	NONE,
	SELECT
};

enum StatusType
{
	Error, WakeUp, Wait
};

enum Result
{
	Failed, Success
};

struct AwaitableResult
{
	StatusType type = Wait;
};

struct AwaitableData
{
	AwaitableData(WorkerType t, UID_t&& event_id) : type(t), EventID(std::move(event_id)), continuation(std::noop_coroutine()), result{} {}

	UID_t EventID;
	std::coroutine_handle<> continuation;
	AwaitableResult result;
	WorkerType type;
};
