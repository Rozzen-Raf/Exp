#pragma once
#include "Concepts.h"
#include "Task.h"
template<class Task = std::coroutine_handle<>>
class ThreadPool {
    using mutex_t = std::mutex; 
    using lock_t = std::unique_lock<mutex_t>;
public:
    using TaskType = Task;

    explicit ThreadPool(size_t threads);

    void Run();

    void AddTask(Task handle);

    ~ThreadPool();
private:
    std::vector< std::thread > workers;
    std::queue< Task > tasks;

    mutex_t queue_mutex;
    std::condition_variable condition;
    bool stop;
};
//---------------------------------------------------------------------

template<class T>
void ThreadPool<T>::AddTask(T handle)
{
    {
        lock_t lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace(handle);
    }
    condition.notify_one();
}
//---------------------------------------------------------------------
template<class Task>
ThreadPool<Task>::ThreadPool(size_t threads) : stop(false)
{
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(
            [this]
            {
                Run();
            }
            );
}
//---------------------------------------------------------------------

template<class Task>
void ThreadPool<Task>::Run()
{
    for (;;)
    {
        Task coro;
        {
            lock_t lock(this->queue_mutex);
            this->condition.wait(lock,
                [this] { return this->stop || !this->tasks.empty(); });
            if (this->stop && this->tasks.empty())
                return;
            coro = std::move(this->tasks.front());
            this->tasks.pop();
        }

        coro.resume();
    }
}
//---------------------------------------------------------------------

template<class Task>
ThreadPool<Task>::~ThreadPool()
{
    {
        lock_t lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}
//---------------------------------------------------------------------

