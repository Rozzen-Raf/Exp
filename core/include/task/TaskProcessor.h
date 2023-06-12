#pragma once
#include "Types.h"

namespace task{
class TaskProcessorContext;
template<class T>
class ThreadPool;

using ProcessorWeakPtr = std::weak_ptr<TaskProcessorContext> ;
using ProcessorSharedPtr = std::shared_ptr<TaskProcessorContext> ; 

class TaskProcessorContext
{
protected:

	virtual ~TaskProcessorContext() = default;
public:

    virtual void AddTask(std::any task) = 0;
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

    virtual void AddTask(std::any task)
	{
        auto handle = std::any_cast<typename TaskProcessor::TaskType>(task);
        Processor->AddTask(handle);
	}

	virtual void Run() final
	{
		Processor->Run();
	}
};
//---------------------------------------------------------------------
}
