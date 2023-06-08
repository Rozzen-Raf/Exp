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

using func_t = std::function<void()> ;
using String = std::string;
using StringView = std::string_view;
using ID_t = int;
//using UID_t = std::variant<String, ID_t, int64_t>;
using UID_t = int64_t;

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

#define LOG(a, m) std::cout << #a << ": " << m << std::endl;

#define ERROR(a,m) std::cerr << #a << ": " << m << std::endl;

using buffer_view = std::span<std::byte>;
using buffer_view_const = std::span<const std::byte>;
using buffer = std::vector<std::byte>;
using buffer_ptr = std::shared_ptr<buffer>;

buffer_ptr ConvertStringToBuffer(const String& string);
ID_t GetLocalTime();
