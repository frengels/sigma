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
struct SignalTraits;

template<bool Nothrow, typename Ret, typename... Args>
struct SignalTraits<Nothrow, Ret(Args...)>
{
    static constexpr bool is_nothrow = Nothrow;

    using return_type = Ret;

    using _base_signature_type = return_type(Args...);

    // read as sigma::FunctionRef<return_type(Args...) noexcept(is_nothrow)>
    using function_type = sigma::FunctionRef<
        std::conditional_t<is_nothrow,
                           sigma::AddSignatureNothrowT<_base_signature_type>,
                           _base_signature_type>>;
};

template<typename Signature>
class Signal final {
    static_assert(sigma::IsSignatureV<Signature>,
                  "Provided Signature is not a valid function signature");

    static_assert(!sigma::IsConstSignatureV<Signature>,
                  "Signature cannot have const qualifier");
    static_assert(!sigma::IsVolatileSignatureV<Signature>,
                  "Signature cannot have volatile qualifier");
    static_assert(!sigma::IsLvalueSignatureV<Signature>,
                  "Signature cannot have lvalue qualifier");
    static_assert(!sigma::IsRvalueSignatureV<Signature>,
                  "Signature cannot have rvalue qualifier");

public:
    using return_type    = sigma::SignatureReturnT<Signature>;
    using signature_type = Signature;
    using function_type  = sigma::FunctionRef<signature_type>;
    using container_type = std::vector<function_type>;
    using mutex_type     = sigma::DummyMutex;

private:
    container_type m_slots;
    mutex_type     m_mutex;

public:
    Signal() = default;

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
    std::vector<sigma::Result<return_type, false>>
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