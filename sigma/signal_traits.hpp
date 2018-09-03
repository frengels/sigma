#ifndef SIGMA_SIGNAL_TRAITS_HPP_INCLUDED
#define SIGMA_SIGNAL_TRAITS_HPP_INCLUDED

#pragma once

#include <functional>

#include "sigma/function_ref.hpp"

namespace sigma
{
template<typename...>
struct slot_traits;

template<typename Sig>
struct slot_traits<sigma::function_ref<Sig>>
{
    using slot_type = sigma::function_ref<Sig>;

    template<typename... Args>
    static constexpr std::
        enable_if_t<std::is_constructible_v<slot_type, Args&&...>, slot_type>
        create(Args&&... args) noexcept(
            std::is_nothrow_constructible_v<slot_type, Args&&...>)
    {
        return sigma::function_ref<Sig>(std::forward<Args>(args)...);
    }

    template<auto Fn, typename T>
    static constexpr std::enable_if_t<
        (sigma::is_member_function_pointer_v<decltype(Fn)> ||
         sigma::is_function_v<decltype(Fn)>) &&!std::is_rvalue_reference_v<T&&>,
        slot_type>
    create(T&& obj) noexcept
    {
        return slot_type::bind<Fn>(std::forward<T>(obj));
    }
};

/*
struct signal_traits
{
    using slot_traits = slot_traits
};
*/
} // namespace sigma

#endif