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

typedef std::function<void()> func_t;
typedef std::string String;
typedef int ID_t;
typedef std::variant<String, ID_t> UID_t;

enum WorkerType
{
	NONE,
	EPOLL
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
    AwaitableData(WorkerType t, UID_t&& event_id) : EventID(std::move(event_id)), continuation(std::noop_coroutine()), type(t), result{} {}

	UID_t EventID;
	std::coroutine_handle<> continuation;
	AwaitableResult result;
	WorkerType type;
};

#define DECLARE_SHARED_PTR(T) class T; using T##SharedPtr = std::shared_ptr<T>;

#define DECLARE_WEAK_PTR(T) class T;\
    using T##WeakPtr = std::weak_ptr<T>;

#define DECLARE_STR_SHARED_PTR(T)    struct T;\
    typedef std::shared_ptr<T> T##SharedPtr;

#define DECLARE_UNIQUE_PTR(T) class T;\
    using T##UniquePtr = std::unique_ptr<T>;

#define SLOG(a, m) \
std::cout << #a << ": " << m << std::endl;
