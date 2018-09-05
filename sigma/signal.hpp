#ifndef SIGMA_SIGNAL_HPP_INCLUDED
#define SIGMA_SIGNAL_HPP_INCLUDED

#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <mutex>

#include "sigma/connection.hpp"
#include "sigma/disconnector.hpp"
#include "sigma/dummy_mutex.hpp"
#include "sigma/function_ref.hpp"
#include "sigma/handle_vector.hpp"
#include "sigma/result.hpp"
#include "sigma/signal_traits.hpp"

namespace sigma
{
template<typename Traits, typename Signature>
class signal_type final {
    static_assert(Traits::template valid_signature<Signature>(),
                  "This traits type does not accept this function signature");

public:
    using return_type    = sigma::signature_return_t<Signature>;
    using signature_type = Signature;
    // using slot_type      = sigma::function_ref<signature_type>;
    using traits_type = Traits;
    using slot_type = typename traits_type::template slot_type<signature_type>;
    using container_type =
        typename traits_type::template container_type<slot_type>;
    using handle_type = typename traits_type::template handle_type<slot_type>;
    // using container_type = sigma::handle_vector<slot_type>;
    using mutex_type = typename traits_type::mutex_type;

    using connection_type = sigma::connection_type<handle_type>;

    struct disconnector : public sigma::disconnector_base<handle_type>
    {
    private:
        std::reference_wrapper<sigma::signal_type<Traits, Signature>> m_signal;

    public:
        disconnector(sigma::signal_type<Traits, Signature>& signal)
            : m_signal{signal}
        {}

        bool is_alive(const connection_type& conn) const noexcept override
        {
            return m_signal.get().connection_alive(conn);
        }

        void operator()(connection_type& c) override
        {
            m_signal.get().disconnect(c);
        }
    };

private:
    container_type m_slots;
    mutex_type     m_mutex;

    std::shared_ptr<disconnector> m_disconnector;

public:
    signal_type() : m_disconnector{std::make_shared<disconnector>(*this)}
    {}

    /**
     * call each slot
     */
    /*
    template<typename RetFn,
             typename ExcFn,
             typename... Args,
             typename = std::enable_if_t<
                 std::is_invocable_r_v<return_type, slot_type, Args&&...> &&
                 std::is_invocable_v<RetFn, return_type> &&
                 std::is_invocable_v<ExcFn, std::exception_ptr>>>
    void operator()(RetFn rfn, ExcFn efn, Args&&... args) noexcept
    {
        std::lock_guard lock{m_mutex};

        for (auto& slot : m_slots)
        {
            if constexpr (std::is_nothrow_invocable_v<slot_type, Args&&...>)
            {
                rfn(std::forward<return_type>(std::invoke(slot,
    std::forward<Args>(args)...));
            }
            else
            {
                try
                {
                    rfn(std::forward<return_type>(
                        std::invoke(slot, std::forward<Args>(args)...)));
                }
                catch (...)
                {
                    efn(std::current_exception());
                }
            }
        }
    }
    */

    template<typename... Args,
             typename = std::enable_if_t<
                 std::is_invocable_r_v<return_type, slot_type, Args&&...>>>
    void operator()(Args&&... args) noexcept
    {
        std::lock_guard lock{m_mutex};

        for (auto& slot : m_slots)
        {
            if (!traits_type::execute_slot(slot))
            {
                continue;
            }

            if constexpr (std::is_nothrow_invocable_v<slot_type, Args&&...>)
            {
                std::invoke(slot, std::forward<Args>(args)...);
            }
            else
            {
                try
                {
                    std::invoke(slot, std::forward<Args>(args)...);
                }
                catch (...)
                {
                    // efn(std::current_exception());
                }
            }
        }
    }

    template<typename... Args>
    std::vector<
        sigma::result<return_type,
                      std::is_nothrow_invocable_v<slot_type, Args&&...>>>
    emit_accumulate(Args&&... args) noexcept
    {
        static_assert(!std::is_same_v<void, return_type>,
                      "Function returns void, no point accumulating results");
        std::vector<
            sigma::result<return_type,
                          std::is_nothrow_invocable_v<slot_type, Args&&...>>>
            accumulator;
        accumulator.reserve(std::size(m_slots));

        (*this)(
            [&](return_type&& ret) {
                accumulator.emplace_back(std::forward<return_type>(ret));
            },
            [&](const std::exception_ptr& ptr) {
                accumulator.emplace_back(ptr);
            },
            std::forward<Args>(args)...);

        return accumulator;
    }

    template<typename... Args>
    sigma::connection_type<handle_type> connect(Args&&... args) noexcept(
        noexcept(traits_type::emplace_slot(m_slots,
                                           std::forward<Args>(args)...)))
    {
        static_assert(std::is_constructible_v<slot_type, Args&&...>,
                      "Cannot construct slot from Args...");
        std::lock_guard lock{m_mutex};

        auto handle =
            traits_type::emplace_slot(m_slots, std::forward<Args>(args)...);

        return connection_type{m_disconnector, handle};
    }

    bool connection_alive(const connection_type& c) const
        noexcept(noexcept(traits_type::validate_handle(m_slots, c.handle())))
    {
        std::lock_guard lock{m_mutex};
        return traits_type::validate_handle(m_slots, c.handle());
    }

    /**
     * usually there's no need to call this function directly, disconnection
     * should be done through the connection's disconnect function
     */
    void disconnect(connection_type& c) noexcept(
        noexcept(traits_type::erase_handle(m_slots, c.handle())))
    {
        std::lock_guard lock{m_mutex};
        traits_type::erase_handle(m_slots, c.handle());
    }
};

template<typename Signature>
using signal = signal_type<sigma::default_signal_traits, Signature>;
} // namespace sigma

#endif