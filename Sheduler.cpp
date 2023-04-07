#include "Sheduler.h"

CoroTaskVoid Loop()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
	co_return;
}