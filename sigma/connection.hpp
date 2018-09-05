#ifndef SIGMA_CONNECTION_HPP_INCLUDED
#define SIGMA_CONNECTION_HPP_INCLUDED

#pragma once

#include <memory>

#include "sigma/disconnector.hpp"
#include "sigma/handle_vector.hpp"

namespace sigma
{
template<typename H>
class connection_type {
public:
    template<typename Traits, typename Sig>
    friend class signal_type;

    using handle_type = H;

private:
    std::weak_ptr<sigma::disconnector_base<handle_type>> m_disconnector;
    handle_type                                          m_handle;
    // some weak reference to the signal_base : std_enable_shared_from_this
private:
    // a connection can only be created by the signal
    connection_type(const std::weak_ptr<sigma::disconnector_base<handle_type>>&
                                       disconnector,
                    const handle_type& handle)
        : m_disconnector{disconnector}, m_handle{handle}
    {}

public:
    connection_type(const connection_type&) noexcept = default;
    connection_type& operator=(const connection_type&) noexcept = default;

    connection_type(connection_type&&) noexcept = default;
    connection_type& operator=(connection_type&&) noexcept = default;

    operator bool() const noexcept
    {
        return alive();
    }

    bool alive() const noexcept
    {
        auto disc = m_disconnector.lock();

        if (disc)
        {
            return disc->is_alive(*this);
        }

        return false;
    }

    void disconnect() noexcept(false)
    {
        auto d = m_disconnector.lock();

        if (d)
        {
            // attempt disconnect
            (*d)(*this);
        }
    }

private:
    inline handle_type handle() const noexcept
    {
        return m_handle;
    }
};
} // namespace sigma

#endif