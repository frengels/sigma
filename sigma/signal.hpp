#ifndef SIGMA_SIGNAL_HPP_INCLUDED
#define SIGMA_SIGNAL_HPP_INCLUDED

#pragma once

#include <functional>
#include <iterator>
#include <mutex>

#include "sigma/dummy_mutex.hpp"
#include "sigma/function_ref.hpp"
#include "sigma/result.hpp"

namespace sigma
{

template<bool Nothrow, typename Signature>
struct signal_traits;

template<bool Nothrow, typename Ret, typename... Args>
struct signal_traits<Nothrow, Ret(Args...)>
{
    static constexpr bool is_nothrow = Nothrow;

    using return_type = Ret;

    using _base_signature_type = return_type(Args...);

    // read as sigma::function_ref<return_type(Args...) noexcept(is_nothrow)>
    using function_type = sigma::function_ref<
        std::conditional_t<is_nothrow,
                           sigma::add_signature_nothrow_t<_base_signature_type>,
                           _base_signature_type>>;
};

template<typename Signature>
class signal final {
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

public:
    using return_type    = sigma::signature_return_t<Signature>;
    using signature_type = Signature;
    using function_type  = sigma::function_ref<signature_type>;
    using container_type = std::vector<function_type>;
    using mutex_type     = sigma::dummy_mutex;

private:
    container_type m_slots;
    mutex_type     m_mutex;

public:
    signal() = default;

    /**
     * call each slot and apply -ret_func- to the returned value
     */
    // TODO SFINAE for invocable on ReturnF
    template<typename... Args>
    void operator()(Args&&... args) noexcept
    {
        static_assert(std::is_invocable_v<function_type, Args&&...>,
                      "Cannot invoke function_type with these arguments");
        std::lock_guard lock{m_mutex};

        for (auto& slot : m_slots)
        {
            try
            {
                // TODO ret_func(std::invoke(slot,
                std::invoke(slot, std::forward<Args>(args)...);
            }
            catch (...)
            {
                // do nothing on exception
            }
        }
    }
    /*
        template<typename... Args>
        void operator()(Args&&... args) noexcept
        {
            (*this)([](auto) {}, std::forward<Args>(args)...);
        }
        */

    template<typename... Args>
    std::vector<sigma::result<return_type, false>>
    emit_accumulate(Args&&... args)
    {
        std::vector<return_type> accumulator;
        accumulator.reserve(std::size(m_slots));

        (*this)(
            [&](return_type&& ret) {
                accumulator.emplace_back(std::forward<return_type>(ret));
            },
            std::forward<Args>(args)...);

        return accumulator;
    }

    template<typename T>
    void connect(T&& callable)
    {
        static_assert(
            std::is_constructible_v<typename container_type::value_type, T&&>,
            "Cannot construct container_type::value_type from T");
        std::lock_guard lock{m_mutex};
        m_slots.emplace_back(std::forward<T>(callable));
    }
};
} // namespace sigma

#endif