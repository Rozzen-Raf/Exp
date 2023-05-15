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

class SingleThreadPolicy
{
public:
    void lock(){}
    void unlock(){}
};

template<typename mutex_t = std::recursive_mutex>
class MultiThreadPolicy
{
public:
    void lock()
    {
        mutex.lock();
    }
    void unlock()
    {
        mutex.unlock();
    }
private:
    mutex_t mutex;
};

template<typename ThreadPolicy>
class PoolAllocator{
private:
    struct  FreeHeader{
    };
    using Node = StackLinkedList<FreeHeader>::Node;
    StackLinkedList<FreeHeader> m_freeList;

    std::size_t m_totalSize;
    std::size_t m_used;   
    std::size_t m_peak;

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

    ThreadPolicy Policy;
};

template<typename ThreadPolicy>
PoolAllocator<ThreadPolicy>::PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize): m_totalSize{totalSize}, m_used { 0 }, m_peak { 0 }  {
    assert(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
    assert(totalSize % chunkSize == 0 && "Total Size must be a multiple of Chunk Size");
    this->m_chunkSize = chunkSize;
}

template<typename ThreadPolicy>
void PoolAllocator<ThreadPolicy>::Init() {
    m_start_ptr = malloc(m_totalSize);
    this->Reset();
}

template<typename ThreadPolicy>
PoolAllocator<ThreadPolicy>::~PoolAllocator() {
    free(m_start_ptr);
}

template<typename ThreadPolicy>
void *PoolAllocator<ThreadPolicy>::Allocate(const std::size_t allocationSize, const std::size_t alignment) {

    std::lock_guard<ThreadPolicy> g(Policy);
    assert(allocationSize == this->m_chunkSize && "Allocation size must be equal to chunk size");

    Node * freePosition = m_freeList.pop();

    assert(freePosition != nullptr && "The pool allocator is full");

    m_used += m_chunkSize;
    m_peak = std::max(m_peak, m_used);
#ifdef _DEBUG
    std::cout << "A" << "\t@S " << m_start_ptr << "\t@R " << (void*) freePosition << "\tM " << m_used << std::endl;
#endif

    return (void*) freePosition;
}

template<typename ThreadPolicy>
void PoolAllocator<ThreadPolicy>::Free(void * ptr) {
    std::lock_guard<ThreadPolicy> g(Policy);
    m_used -= m_chunkSize;

    m_freeList.push((Node *) ptr);

#ifdef _DEBUG
    std::cout << "F" << "\t@S " << m_start_ptr << "\t@F " << ptr << "\tM " << m_used << std::endl;
#endif
}

template<typename ThreadPolicy>
void PoolAllocator<ThreadPolicy>::Reset() {
    std::lock_guard<ThreadPolicy> g(Policy);
    m_used = 0;
    m_peak = 0;
    // Create a linked-list with all free positions
    const int nChunks = m_totalSize / m_chunkSize;
    for (int i = 0; i < nChunks; ++i) {
        std::size_t address = (std::size_t) m_start_ptr + i * m_chunkSize;
        m_freeList.push((Node *) address);
    }
}

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
