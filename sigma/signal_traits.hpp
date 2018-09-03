#ifndef SIGMA_SIGNAL_TRAITS_HPP_INCLUDED
#define SIGMA_SIGNAL_TRAITS_HPP_INCLUDED

#pragma once

#include <functional>

#include "sigma/dummy_mutex.hpp"
#include "sigma/function_ref.hpp"

namespace sigma
{

struct default_signal_traits
{
    template<typename T>
    using slot_type = sigma::function_ref<T>;
    template<typename T>
    using container_type = sigma::handle_vector<T>;
    template<typename T>
    using handle_type = typename container_type<T>::handle_type;
    using mutex_type  = sigma::dummy_mutex;

    template<typename T>
    static constexpr bool validate_handle(const container_type<T>& c,
                                          const handle_type<T>&    h) noexcept
    {
        return c.is_valid_handle(h);
    }
};

} // namespace sigma

#endif