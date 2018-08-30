#pragma once

#include <functional>
#include <iterator>
#include <mutex>

#include "sigma/dummy_mutex.hpp"
#include "sigma/result.hpp"

namespace sigma
{
template<typename Ret, typename... Args>
class Signal;

template<typename Ret, typename... Args>
class Signal<Ret(Args...)> {
public:
    using return_type    = Ret;
    using function_type  = std::function<return_type(Args...)>;
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

    void connect(function_type&& f)
    {
        auto insert_it = std::back_inserter(m_slots);
        insert_it      = std::move(f);
    }

    void connect(const function_type& f)
    {
        auto insert_it = std::back_inserter(m_slots);
        insert_it      = f;
    }
};
} // namespace sigma