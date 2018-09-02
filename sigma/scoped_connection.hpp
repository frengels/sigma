#ifndef SIGMA_SCOPED_CONNECTION_HPP_INCLUDED
#define SIGMA_SCOPED_CONNECTION_HPP_INCLUDED

#pragma once

#include "sigma/connection.hpp"

namespace sigma
{
class scoped_connection {
private:
    sigma::connection m_con;

public:
    /**
     * move to indicate we are transferring ownership to this object
     */
    scoped_connection(connection&& c) : m_con{std::move(c)}
    {}

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