#ifndef SIGMA_SIGNAL_TRAITS_HPP_INCLUDED
#define SIGMA_SIGNAL_TRAITS_HPP_INCLUDED

#pragma once

#include <functional>
#include <mutex>
#include <vector>

#include "sigma/dummy_mutex.hpp"
#include "sigma/function_ref.hpp"
#include "sigma/handle_vector.hpp"

namespace sigma
{

struct default_signal_traits
{
    template<typename Sig>
    using slot_type = sigma::function_ref<Sig>;
    template<typename Slot>
    using container_type = sigma::handle_vector<Slot>;
    template<typename Slot>
    using handle_type = typename container_type<Slot>::handle_type;
    using mutex_type  = sigma::dummy_mutex;

    template<typename Slot>
    static constexpr bool validate_handle(const container_type<Slot>& c,
                                          const handle_type<Slot>& h) noexcept
    {
        return c.is_valid_handle(h);
    }

    template<typename Slot>
    static constexpr void
    erase_handle(container_type<Slot>&    c,
                 const handle_type<Slot>& h) noexcept(false)
    {
        return c.erase(h);
    }

    template<typename Sig>
    static constexpr bool execute_slot(const slot_type<Sig>& s) noexcept
    {
        return true;
    }
};

struct std_signal_traits
{
    template<typename Sig>
    using slot_type = std::function<Sig>;
    template<typename Slot>
    using container_type = std::vector<Slot>;
    template<typename Slot>
    using handle_type = typename container_type<Slot>::size_type;
    using mutex_type  = std::mutex;

    template<typename Slot>
    static constexpr bool validate_handle(const container_type<Slot>& c,
                                          const handle_type<Slot>& h) noexcept
    {
        return bool(c[h]);
    }

    template<typename Slot>
    static constexpr void erase_handle(container_type<Slot>&    c,
                                       const handle_type<Slot>& h) noexcept
    {
        c[h] = nullptr;
    }

    template<typename Sig>
    static constexpr bool execute_slot(const slot_type<Sig>& s) noexcept
    {
        return s != nullptr;
    }
};

} // namespace sigma

#endif