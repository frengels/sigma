#include "sigma/connection.hpp"

#include "sigma/disconnector.hpp"

namespace sigma
{
bool connection::alive() const noexcept
{
    return !m_disconnector.expired();
}

void connection::disconnect() noexcept(false)
{
    auto d = m_disconnector.lock();

    if (d)
    {
        // attempt disconnect
        (*d)(*this);
    }
}
} // namespace sigma