#include "Task.h"

void task_run(TaskSharedPtr task, DeleterType deleter)
{
	task->deleterFnc = deleter;
	task->taskPtr->set_done_callback(std::bind(&Task::unreg, task.get()));
	task->processor_->AddTask(task);
}