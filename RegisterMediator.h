#include "Types.h"

class RegisterMediatorBase
{
public:
    virtual int Register(std::any reg_instance) noexcept = 0;
    virtual int Unregister(std::any unreg_instance) noexcept = 0;
};

template<typename T>
class RegisterMediator : public RegisterMediatorBase
{
private:
    std::shared_ptr<T> instance;
public:
    explicit RegisterMediator(std::shared_ptr<T> ins) : instance(ins){}

    virtual int Register(std::any reg_instance) noexcept final
    {
        return instance->Register(reg_instance);
    }
    virtual int Unregister(std::any unreg_instance) noexcept final
    {
        return instance->Unregister(unreg_instance);
    }
};
typedef std::shared_ptr<RegisterMediatorBase> RegisterMediatorBasePtr;
