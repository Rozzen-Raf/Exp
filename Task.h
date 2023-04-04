#pragma once
#include "Types.h"
class TaskProcessorContext;
typedef std::weak_ptr<TaskProcessorContext> ProcessorWeakPtr;
typedef std::shared_ptr<TaskProcessorContext> ProcessorSharedPtr; 

class Sheduler;

class Task
{
private:
	struct TaskContext
	{
		virtual ~TaskContext() = default;

		virtual void start() = 0;

		virtual void* get_ptr() noexcept = 0;
		virtual void set_done_callback(func_t callback) noexcept = 0;
	};

	template<typename Task>
	struct TaskModel : TaskContext
	{
		Task instance;
		ProcessorSharedPtr processor;

		explicit TaskModel(Task&& task, ProcessorSharedPtr processor_ptr) : instance(std::move(task)), processor(processor_ptr) 
		{
			
		}

		virtual void start() noexcept final
		{
			processor->AddTask(instance.GetFunc());
		}

		virtual void* get_ptr() noexcept final
		{
			return &instance;
		}

		virtual void set_done_callback(func_t callback) noexcept final
		{
			instance.SetDoneCallback(callback);
		}
	};

	std::unique_ptr<TaskContext> taskPtr;
	typedef std::function<void(Task*)> DeleterType;
	DeleterType deleterFnc;
	UID_t ID;
public:
	template<typename T>
	explicit Task(T&& task, ProcessorSharedPtr processor) noexcept
	{
		ID = uuid::generate_uuid_v4();
		taskPtr  = std::make_unique<TaskModel<T>>(std::move(task), processor);
	}

	void run(DeleterType deleter = DeleterType()) noexcept
	{
		deleterFnc = deleter;

		taskPtr->start();
	}

	void unreg()
	{
		deleterFnc(this);
	}

	const UID_t& GetId() const noexcept { return ID; }
};

class TaskProcessorContext
{
protected:

	virtual ~TaskProcessorContext() = default;
public:

	virtual void AddTask(func_t func) = 0;
};

template<typename TaskProcessor>
class TaskProcessorModel : public TaskProcessorContext
{
private:
	typedef std::unique_ptr<TaskProcessor> TaskProcessorUniquePtr;
	TaskProcessorUniquePtr Processor;

public:
	explicit TaskProcessorModel(TaskProcessorUniquePtr&& instance) noexcept : Processor(std::move(instance))
	{
	}
	TaskProcessorModel(const TaskProcessor& instance) = delete;

	virtual void AddTask(func_t func) noexcept final
	{
		Processor->AddTask(func);
	}
};
