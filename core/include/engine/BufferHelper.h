#pragma once

#include "Types.h"

namespace engine
{
    template<typename ElType>
    class BufferHelperBase
    {
    public:
        using ElementType = ElType;
        using Pointer = ElType*;
        using ConstPointer = const ElementType*;

        BufferHelperBase(buffer_view& view): View(view), Head(0), Tail(view.size()-1)
        {
            ASSERT(view.size());
        }
        
        ConstPointer Peek()
        {
            return ConstPointer(View.data()) + Head;
        }

        ConstPointer begin()
        {
            return ConstPointer(&View.data()[0]);
        }

        ConstPointer end()
        {
            return ConstPointer(&View.data()[Tail]);
        }

        void MoveTo(ConstPointer ptr)
        {
            ASSERT(ptr >= Peek());
            ASSERT(ptr <= end());
            size_t len = ptr - Peek();

            ASSERT(Head + len <= Tail);

            Head += len; 
        }

    private:
        size_t Head;
        size_t Tail;
        buffer_view View;
    };
    using BufferHelper = BufferHelperBase<char>;

    class Buffer
    {
    private:
        buffer mBuffer;
    };
}