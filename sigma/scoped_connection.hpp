#ifndef SIGMA_SCOPED_CONNECTION_HPP_INCLUDED
#define SIGMA_SCOPED_CONNECTION_HPP_INCLUDED

#pragma once

#include "sigma/connection.hpp"

namespace sigma
{
template<typename H>
class scoped_connection {
public:
    using handle_type     = H;
    using connection_type = sigma::connection_type<handle_type>;

private:
    connection_type m_con;

public:
    /**
     * move to indicate we are transferring ownership to this object
     */
    scoped_connection(connection_type&& c) : m_con{std::move(c)}
    {}

    /**
     * scoped_connection is not copyable as it guarantees unique ownership
     */
    scoped_connection(const scoped_connection&) noexcept = delete;
    scoped_connection& operator=(const scoped_connection&) noexcept = delete;

    scoped_connection(scoped_connection&&) noexcept = default;
    scoped_connection& operator=(scoped_connection&&) noexcept = default;

    ~scoped_connection()
    {
        m_con.disconnect();
    }
};
} // namespace sigma

#endif