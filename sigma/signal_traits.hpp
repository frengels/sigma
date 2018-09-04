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

    template<typename Signature>
    static constexpr bool valid_signature()
    {
        static_assert(sigma::is_signature_v<Signature>,
                      "Provided Signature is not a valid function signature");
        static_assert(!sigma::is_signature_const_v<Signature>,
                      "Signature cannot have const qualifier");
        static_assert(!sigma::is_signature_volatile_v<Signature>,
                      "Signature cannot have volatile qualifier");
        static_assert(!sigma::is_signature_lvalue_v<Signature>,
                      "Signature cannot have lvalue qualifier");
        static_assert(!sigma::is_signature_rvalue_v<Signature>,
                      "Signature cannot have rvalue qualifier");

        return true;
    }

    template<typename Slot>
    static constexpr bool validate_handle(
        const container_type<Slot>& c,
        const handle_type<Slot>&    h) noexcept(noexcept(c.is_valid_handle(h)))
    {
        return c.is_valid_handle(h);
    }

    template<typename Slot>
    static constexpr void
    erase_handle(container_type<Slot>&    c,
                 const handle_type<Slot>& h) noexcept(noexcept(c.erase(h)))
    {
        return c.erase(h);
    }

    template<typename Sig, typename... Args>
    static constexpr handle_type<slot_type<Sig>>
    emplace_slot(container_type<slot_type<Sig>>& c, Args&&... args) noexcept(
        noexcept(c.emplace_back(std::forward<Args>(args)...)))
    {
        auto handle = c.emplace_back(std::forward<Args>(args)...);
        return handle;
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

    template<typename Signature>
    static constexpr bool valid_signature()
    {
        static_assert(sigma::is_signature_v<Signature>,
                      "Provided Signature is not a valid function signature");
        static_assert(!sigma::is_signature_const_v<Signature>,
                      "Signature cannot have const qualifier");
        static_assert(!sigma::is_signature_volatile_v<Signature>,
                      "Signature cannot have volatile qualifier");
        static_assert(!sigma::is_signature_lvalue_v<Signature>,
                      "Signature cannot have lvalue qualifier");
        static_assert(!sigma::is_signature_rvalue_v<Signature>,
                      "Signature cannot have rvalue qualifier");
        static_assert(!sigma::is_signature_nothrow_v<Signature>,
                      "Signature cannot have nothrow qualifier");

        return true;
    }

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

    template<typename Sig, typename... Args>
    static constexpr handle_type<slot_type<Sig>>
    emplace_slot(container_type<slot_type<Sig>>& c, Args&&... args) noexcept(
        noexcept(std::function<Sig>{std::forward<Args>(args)...}))
    {
        for (auto it = c.begin(); it != c.end(); ++it)
        {
            if (!(*it))
            {
                *it         = std::function<Sig>{std::forward<Args>(args)...};
                auto handle = std::distance(c.begin(), it);
                return static_cast<handle_type<slot_type<Sig>>>(handle);
            }
        }
    }

    template<typename Sig>
    static constexpr bool execute_slot(const slot_type<Sig>& s) noexcept
    {
        return s != nullptr;
    }
};

} // namespace sigma

#endif