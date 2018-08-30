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
};

template<typename Ret, typename... Args>
class Signal;

template<typename Ret, typename... Args>
class Signal<Ret(Args...)> {
public:
    using return_type    = Ret;
    using function_type  = std::function<Ret(Args...)>;
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
    template<typename ReturnF>
    void operator()(ReturnF ret_func, Args... args) noexcept
    {
        std::lock_guard lock{m_mutex};

        for (auto& slot : m_slots)
        {
            try
            {
                // TODO ret_func(std::invoke(slot,
                // std::forward<Args>(args)...));
                std::invoke(slot, std::forward<Args>(args)...);
            }
            catch (...)
            {
                // do nothing on exception
            }
        }
    }

    void operator()(Args... args) noexcept
    {
        (*this)([](auto) {}, std::forward<Args>(args)...);
    }

    std::vector<return_type> emit_accumulate(Args... args)
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

    template<
        typename T,
        typename = std::enable_if_t<
            std::is_constructible_v<typename container_type::value_type, T&&>>>
    void connect(T&& callable)
    {
        m_slots.emplace_back(std::forward<T>(callable));
    }
};
} // namespace sigma