#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "Types.h"

template <class T>
class StackLinkedList {
public:
    struct Node {
        T data;
        Node* next;
    };
    
    Node* head;
public:
    StackLinkedList() = default;
    StackLinkedList(StackLinkedList &stackLinkedList) = delete;
    void push(Node * newNode);
    Node* pop();
};

template <class T>
void StackLinkedList<T>::push(Node * newNode) {
    newNode->next = head;
    head = newNode;
}

template <class T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
    Node * top = head;
    head = head->next;
    return top;
}

class PoolAllocator{
private:
    struct  FreeHeader{
    };
    using Node = StackLinkedList<FreeHeader>::Node;
    StackLinkedList<FreeHeader> m_freeList;

    void * m_start_ptr = nullptr;
    std::size_t m_chunkSize;
public:
    PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize);

    virtual ~PoolAllocator();

    void* Allocate(const std::size_t size, const std::size_t alignment = 0) ;

    void Free(void* ptr);

    void Init();

    void Reset();
private:
    PoolAllocator(PoolAllocator &poolAllocator);

    std::size_t m_totalSize;
    std::size_t m_used;   
    std::size_t m_peak;
};

template<typename T, class Allocator, typename... Args>
std::shared_ptr<T> make_custom_shared(Allocator& allocator, Args&&... args)
{
    const auto custom_deleter = [&allocator](T* ptr) mutable
    {
        ptr->~T();
        allocator.Free(ptr);
    };

    void* memory_block = allocator.Allocate(sizeof(T), alignof(T));
    if (memory_block)
    {
        T* object_block = static_cast<T*>(memory_block);
        new (reinterpret_cast<void*>(object_block)) T { std::forward<Args>(args)... };
        return std::shared_ptr<T>(object_block, custom_deleter);
    }

    return nullptr;
}

#endif // ALLOCATOR_H
