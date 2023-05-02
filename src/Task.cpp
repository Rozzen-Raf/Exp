#include "Task.h"
//---------------------------------------------------------------------

void task_run(TaskSharedPtr task, DeleterType deleter)
{
	task->deleterFnc = deleter;
    task->taskPtr->set_done_callback([task = task.get()](){task->unreg();});
	task->processor_->AddTask(task);
}
//---------------------------------------------------------------------
