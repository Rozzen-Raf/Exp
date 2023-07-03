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
#include <concepts>
#include "UID.h"
#include <string.h>
#include <any>
#include <csignal>
#include <span>
#include <optional>
#include <fstream>
#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>
#ifdef __linux__
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#endif

#define DEBUG 1

#ifdef DEBUG
#define ASSERT(a) assert(a)
#elif
#define ASSERT(a)
#endif

namespace task{
template<class T>
class CoroTask;
}

namespace engine
{
using func_t = std::function<void()> ;
using String = std::string;
using StringView = std::string_view;
using ID_t = int;
using UID_t = int64_t;
using buffer_view = std::span<std::byte>;
using buffer_view_const = std::span<const std::byte>;
using buffer = std::vector<std::byte>;
using buffer_ptr = std::shared_ptr<buffer>;
using handler_packet_f = std::function<task::CoroTask<String>(buffer_view&)>;
enum WorkerType
{
	NONE,
	EPOLL
};

enum StatusType
{
	Error, WakeUp, Wait, HangUp, Success
};

struct AwaitableResult
{
	StatusType type = Wait;
    UID_t id;
	String err_message;
	int err;

    constexpr operator bool() const { return type == WakeUp || type == Success;}
};

struct AwaitableData
{
    AwaitableData(WorkerType t, UID_t&& event_id) : EventID(std::move(event_id)), continuation(std::noop_coroutine()), type(t), result{} {}

	UID_t EventID;
	std::coroutine_handle<> continuation;
	AwaitableResult result;
	WorkerType type;
	bool NeedUnreg = true;
};

#define DECLARE_SHARED_PTR(T) class T; using T##SharedPtr = std::shared_ptr<T>;

#define DECLARE_WEAK_PTR(T) class T;\
    using T##WeakPtr = std::weak_ptr<T>;

#define DECLARE_STR_SHARED_PTR(T)    struct T;\
    using T##SharedPtr = std::shared_ptr<T>;

#define DECLARE_UNIQUE_PTR(T) class T;\
    using T##UniquePtr = std::unique_ptr<T>;

#define UNIQUE_PTR(T) using T##UniquePtr = std::unique_ptr<T>;

#define SHARED_PTR(T) using T##SharedPtr = std::shared_ptr<T>;

#define LOG(a, m) std::cout << #a << ": " << m << std::endl;

#define ERROR(a,m) std::cerr << #a << ": " << m << std::endl;

buffer_ptr ConvertStringToBuffer(const String& string);
ID_t GetLocalTime();
}

using func_t = engine::func_t;
using String = engine::String;
using StringView = engine::StringView;
using ID_t = engine::ID_t;
using UID_t = engine::UID_t;
using buffer_view = engine::buffer_view;
using buffer_view_const = engine::buffer_view_const;
using buffer = engine::buffer;
using buffer_ptr = engine::buffer_ptr;
using handler_packet_f = engine::handler_packet_f;