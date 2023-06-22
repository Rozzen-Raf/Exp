#pragma once

#include "Types.h"

namespace engine
{
    static constexpr char CRLF[]{"\r\n"};

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
        
        ConstPointer Peek() const noexcept
        {
            return ConstPointer(View.data()) + Head;
        }

        ConstPointer begin() const noexcept
        {
            return ConstPointer(&View.data()[0]);
        }

        ConstPointer end() const noexcept
        {
            return ConstPointer(&View.data()[Tail]);
        }

        ConstPointer FindCRLF() const noexcept
        {
            ConstPointer crlf = std::search(Peek(), end(), CRLF, CRLF + 2);
            return crlf == end() ? nullptr : crlf;
        }

        void MoveTo(ConstPointer ptr) noexcept
        {
            ASSERT(ptr >= Peek());
            ASSERT(ptr <= end());
            size_t len = ptr - Peek();

            MoveOn(len);
        }

        void MoveOn(size_t length) noexcept
        {
            ASSERT(Head + length <= Tail);

            Head += length; 
        }

        size_t Size() const noexcept{
            return Head - Tail;
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