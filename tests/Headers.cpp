#include "Headers.h"

ShedulerSharedPtr GetSheduler()
{
    static ShedulerSharedPtr sheduler;

    if(!sheduler)
    {
        ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool>>(8);
		sheduler = std::make_shared<Sheduler>(processor);
    }

    return sheduler;
}