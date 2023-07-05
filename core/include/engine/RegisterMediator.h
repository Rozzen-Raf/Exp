#pragma once
#include "Types.h"
#include "Concepts.h"
namespace engine
{
class RegisterMediatorBase
{
public:
    virtual int Register(const std::any& reg_instance) noexcept = 0;
    virtual int Unregister(const std::any& unreg_instance) noexcept = 0;
    virtual int Type() const noexcept = 0;
};

template<typename T>
class RegisterMediator : public RegisterMediatorBase
{
private:
    std::shared_ptr<T> instance;
public:
    explicit RegisterMediator(std::shared_ptr<T> ins) : instance(ins){}

    virtual int Register(const std::any& reg_instance) noexcept final
    {
        if constexpr(HasRegister<T, decltype(reg_instance)>)
        {
            return instance->Register(reg_instance);
        }
        else
        {
            return -1;
        }
    }
    virtual int Unregister(const std::any& unreg_instance) noexcept final
    {
        if constexpr(HasUnregister<T, decltype(unreg_instance)>)
        {
            return instance->Unregister(unreg_instance);
        }
        else
        {
            return -1;
        }
    }
    virtual int Type() const noexcept final
    {
        return T::Type;
    }
};
typedef std::shared_ptr<RegisterMediatorBase> RegisterMediatorBasePtr;
}
