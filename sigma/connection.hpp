#ifndef SIGMA_CONNECTION_HPP_INCLUDED
#define SIGMA_CONNECTION_HPP_INCLUDED

#pragma once

#include <memory>

#include "sigma/disconnector.hpp"
#include "sigma/handle_vector.hpp"

namespace sigma
{
class disconnector_base;

class connection {
public:
    template<typename Sig>
    friend class signal;

    using handle_type = sigma::vector_handle;

private:
    std::weak_ptr<sigma::disconnector_base> m_disconnector;
    handle_type                             m_handle;
    // some weak reference to the signal_base : std_enable_shared_from_this
private:
    // a connection can only be created by the signal
    connection(const std::weak_ptr<sigma::disconnector_base>& disconnector,
               const sigma::vector_handle                     handle)
        : m_disconnector{disconnector}, m_handle{handle}
    {}

public:
    connection(const connection&) noexcept = default;
    connection& operator=(const connection&) noexcept = default;

    connection(connection&&) noexcept = default;
    connection& operator=(connection&&) noexcept = default;

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