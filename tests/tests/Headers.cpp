#include "Headers.h"

task::ShedulerSharedPtr GetSheduler()
{
    static task::ShedulerSharedPtr sheduler;

    if(!sheduler)
    {
        task::ProcessorSharedPtr processor = std::make_shared<task::TaskProcessorModel<task::ThreadPool<std::coroutine_handle<>>>>(8);
		sheduler = std::make_shared<task::Sheduler>(processor);
    }

    return sheduler;
}