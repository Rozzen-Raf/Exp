#pragma once
#include "Types.h"
struct AwaitableFinalContinuation {

    constexpr bool await_ready() const noexcept { return false; }
   std::coroutine_handle<>
        await_suspend(auto caller) const noexcept {
        auto handle = caller.promise().continuation;

        if (caller.promise().done_callback)
            caller.promise().done_callback();

        return handle;
    }
    constexpr void await_resume() const noexcept {
        
    }
};
