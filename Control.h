#pragma once
#include "Types.h"
class ControlBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) = 0;
};

typedef std::shared_ptr<ControlBase> ControlBaseSharedPtr;

class Control : public ControlBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) final
	{

	}
private:
	std::map<UID_t, AwaitableData*> Awaitables;
};

