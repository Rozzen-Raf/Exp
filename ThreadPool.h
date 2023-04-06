#pragma once
#include "Types.h"
#include "Task.h"
class ThreadPool {
    using mutex_t = std::mutex; 
    using lock_t = std::unique_lock<mutex_t>;
public:
    ThreadPool(size_t threads) : stop(false)
    {
        for (size_t i = 0; i < threads; ++i)
            workers.emplace_back(
                [this]
                {
                    for (;;)
                    {
                        std::function<void()> task;

                        {
                            lock_t lock(this->queue_mutex);
                            this->condition.wait(lock,
                                [this] { return this->stop || !this->tasks.empty(); });
                            if (this->stop && this->tasks.empty())
                                return;
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }

                        task();
                    }
                }
                );
    }

    template<typename task_ptr>
    void AddTask(task_ptr task)
    {
        {
            lock_t lock(queue_mutex);

            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() {
                std::invoke(task->GetFunc());
                });
        }
        condition.notify_one();
    }

    ~ThreadPool()
    {
        {
            lock_t lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }
private:
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;

    mutex_t queue_mutex;
    std::condition_variable condition;
    bool stop;
};