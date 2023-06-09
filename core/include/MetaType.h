#ifndef METATYPE_H
#define METATYPE_H
#include "Types.h"

class IArguments
{
public:
    virtual ~IArguments(){}

    virtual void SetArgs(void* obj) = 0;
};

template<typename T>
class Arguments : public IArguments
{
    class ArgsContext
    {
        public:
        virtual ~ArgsContext(){}
        virtual void SetArgs(T* p) = 0;
    };

    template<typename... Args>
    class ArgsModel : public ArgsContext
    {
        public:
        using Tuple = std::tuple<Args...>;
        Tuple args;
        
        static const size_t length = sizeof...(Args);

        explicit ArgsModel(Args&&... a) : args(std::forward<Args>(a)...)
        {

        }

        virtual void SetArgs(T* object) final
        {
            unpack(object, std::make_index_sequence<sizeof...(Args)>());
        }
        private:
        template<std::size_t ...S>
        void unpack(T* obj, std::index_sequence<S...>)
        {
            obj->SetArgs(std::get<S>(args)...);
        }
    };

    std::unique_ptr<ArgsContext> arg_ptr;
public:
    template<typename... Args>
    explicit Arguments(Args&&... args) : arg_ptr(std::make_unique<ArgsModel<Args...>>(std::forward<Args>(args)...))
    {
    }

    void SetArgs(void* object)
    {
        T* o = static_cast<T*>(object);

        if(o)
            arg_ptr->SetArgs(o);
    }
};

class ICreator
{
public:
    virtual std::shared_ptr<void> Create(IArguments*) const = 0;
};
DECLARE_SHARED_PTR(ICreator)
template<typename T, typename Base = ICreator>
class ObjectCreator : public Base
{
public:
    virtual std::shared_ptr<void> Create(IArguments* args) const final
    {
        auto obj = std::make_shared<T>();
        if(args)
            args->SetArgs(obj.get());
        return std::static_pointer_cast<void>(obj);
    }
};

class MetaType
{
public:
    MetaType() = delete;
    explicit MetaType(int&& index, std::string&& class_name, ICreatorSharedPtr creator) noexcept : Index(index), ClassName(std::move(class_name)), Creator(creator)
    {

    }

    std::shared_ptr<void> Construct(IArguments* args) const
    {
        return Creator->Create(args);
    }

    inline const std::string& GetTypeClassName() const noexcept { return ClassName; }

    inline int GetIndex() const noexcept { return Index; }
private:
    int Index;
    std::string ClassName;
    ICreatorSharedPtr Creator;
};
DECLARE_SHARED_PTR(MetaType)

using SMetaMaps = std::unordered_map<std::string, MetaTypeSharedPtr>;
using PSMetaMaps = std::shared_ptr<SMetaMaps>;
class MetaData;
DECLARE_UNIQUE_PTR(MetaData)
class MetaData
{
public:
    MetaData() : MetaMaps(std::make_shared<SMetaMaps>())
    {
    }

    MetaTypeSharedPtr Register(std::string&& class_name, MetaTypeSharedPtr metatype)
    {
        MetaMaps->insert({ std::move(class_name), metatype });

        return metatype;
    }

    std::shared_ptr<void> Create(const std::string& str, IArguments* args = nullptr)
    {
        auto&& find_iter = MetaMaps->find(str);
        if (find_iter == MetaMaps->end())
            return nullptr;

        return find_iter->second->Construct(args);
    }

    static MetaData* GetMetaData();

private:
    PSMetaMaps MetaMaps;
    static MetaDataUniquePtr StaticMetaData;
};

#define METATYPE \
public:\
static MetaTypeSharedPtr StaticMetaType;\
static inline MetaTypeSharedPtr GetMetaType() noexcept { return StaticMetaType;}\
private:\

#define METATYPE_DEF(T) MetaTypeSharedPtr T::StaticMetaType = nullptr;
#define METATYPE_DEF_TEMPL(T) template<>\
MetaTypeSharedPtr T::StaticMetaType = nullptr;

#define REGISTER_METATYPE(T) T::StaticMetaType = MetaData::GetMetaData()->Register(#T, std::make_shared<MetaType>(0, #T, std::make_shared<ObjectCreator<T>>()));
#define REGISTER_METATYPE_TEMPL(T, templ) T<templ>::StaticMetaType = MetaData::GetMetaData()->Register(#T, std::make_shared<MetaType>(0, #T, std::make_shared<ObjectCreator<T<templ>>>()));
#endif // METATYPE_H
