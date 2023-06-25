#include "catch2/catch_all.hpp"
#include "ThreadPool.h"
#include "Sheduler.h"
#include "MetaTypeInclude.h"
#include "ChainableTask.h"
#include "EpollWorker.h"

task::ShedulerSharedPtr GetSheduler();
