#pragma once
#include "Types.h"

class TaskProcessorContext;
class Task;
class ThreadPool;

using ProcessorWeakPtr = std::weak_ptr<TaskProcessorContext> ;
using ProcessorSharedPtr = std::shared_ptr<TaskProcessorContext> ; 
using TaskSharedPtr = std::shared_ptr<Task>;
using DeleterType = std::function<void(Task*)>;

void task_run(TaskSharedPtr task, DeleterType deleter = DeleterType());

class TaskProcessorContext
{
protected:

	virtual ~TaskProcessorContext() = default;
public:

	virtual void AddTask(TaskSharedPtr task) = 0;
	virtual void Run() = 0;
};

template<typename TaskProcessor>
class TaskProcessorModel : public TaskProcessorContext
{
private:
	typedef std::unique_ptr<TaskProcessor> TaskProcessorUniquePtr;
	TaskProcessorUniquePtr Processor;

public:
	template<typename... Args>
    explicit TaskProcessorModel(Args&&... args) noexcept : Processor(std::make_unique<TaskProcessor>(std::forward<Args>(args)...))
	{

	}
	TaskProcessorModel(const TaskProcessor& instance) = delete;

    virtual void AddTask(TaskSharedPtr func) final
	{
		Processor->AddTask(func);
	}

	virtual void Run() final
	{
		Processor->Run();
	}
};
//---------------------------------------------------------------------

class Task
{
private:
	struct TaskContext
	{
		virtual ~TaskContext() = default;

		virtual func_t get_func() const noexcept = 0;

		virtual void* get_ptr() noexcept = 0;
		virtual void set_done_callback(func_t callback) noexcept = 0;
	};

	template<typename T>
	struct TaskModel : TaskContext
	{
		T instance;

		explicit TaskModel(T&& task) : instance(std::move(task))		{
			
		}

		virtual void* get_ptr() noexcept final
		{
			return &instance;
		}

		virtual void set_done_callback(func_t callback) noexcept final
		{
			instance.SetDoneCallback(callback);
		}

		virtual func_t get_func() const noexcept final
		{
			return instance.GetFunc();
		}	
	};

	std::unique_ptr<TaskContext> taskPtr;
	DeleterType deleterFnc;
	UID_t ID;
	ProcessorSharedPtr processor_;
	friend void task_run(TaskSharedPtr task, DeleterType deleter);
	friend class ThreadPool;
public:
	template<typename T>
	explicit Task(T&& task, ProcessorSharedPtr processor) noexcept : 
		taskPtr(std::make_unique<TaskModel<T>>(std::move(task))), 
		processor_(processor), 
        ID(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
	{
	}

	const UID_t& GetId() const noexcept { return ID; }
private:

	void unreg()
	{
		if(deleterFnc)
			deleterFnc(this);
	}

	auto GetFunc() const noexcept
	{
		return taskPtr->get_func();
	}
};
//---------------------------------------------------------------------
