#ifndef SIGMA_DUMMY_MUTEX_HPP_INCLUDED
#define SIGMA_DUMMY_MUTEX_HPP_INCLUDED

#pragma once

namespace sigma
{
struct dummy_mutex
{
    inline constexpr void lock() const noexcept
    {}

    inline constexpr bool try_lock() const noexcept
    {
        return true;
    }

    inline constexpr void unlock() const noexcept
    {}
};
} // namespace sigma

#endif