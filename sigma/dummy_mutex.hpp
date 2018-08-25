#pragma once

namespace sigma
{
struct DummyMutex
{
    inline constexpr void lock() noexcept
    {}

    inline constexpr bool try_lock() noexcept
    {
        return true;
    }

    inline constexpr void unlock() noexcept
    {}
};
} // namespace sigma