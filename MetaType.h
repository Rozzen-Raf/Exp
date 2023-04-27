#ifndef METATYPE_H
#define METATYPE_H
#include "Types.h"
class ICreator
{
public:
    virtual std::shared_ptr<void*> Create() const = 0;
};
DECLARE_SHARED_PTR(ICreator)
template<typename T, typename Base = ICreator>
class ObjectCreator : public Base
{
public:
    virtual std::shared_ptr<void*> Create() const final
    {
        return std::make_shared<T>();
    }
};

class MetaType
{
public:
    MetaType() = delete;
    explicit MetaType(int&& index, std::string&& class_name, ICreatorSharedPtr creator) noexcept : Index(index), ClassName(std::move(class_name)), Creator(creator)
    {

    }

    std::shared_ptr<void*> Construct() const
    {
        return Creator->Create();
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
    MetaData() 
    {
        MetaMaps = std::make_shared<SMetaMaps>();
    }

    MetaTypeSharedPtr Register(std::string&& class_name, MetaTypeSharedPtr metatype)
    {
        MetaMaps->insert({ std::move(class_name), metatype });

        return metatype;
    }

    std::shared_ptr<void*> Create(const std::string& str)
    {
        auto&& find_iter = MetaMaps->find(str);
        if (find_iter == MetaMaps->end())
            return nullptr;

        return find_iter->second->Construct();
    }

    static MetaData* GetMetaData();

private:
    PSMetaMaps MetaMaps;
    static MetaDataUniquePtr StaticMetaData;
};

#define METATYPE \
public:\
static SMetaTypeSharedPtr StaticMetaType;\
static inline SMetaTypeSharedPtr GetMetaType() noexcept { return StaticMetaType;}\
private:\

#define REGISTER_METATYPE(T) PSMetaType T::StaticMetaType = MetaData::GetMetaData()->Register(#T, std::make_shared<MetaType>(0, #T, std::make_shared<ObjectCreator<T>>()));
#endif // METATYPE_H
