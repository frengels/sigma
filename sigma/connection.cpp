#include "sigma/connection.hpp"

#include "sigma/disconnector.hpp"

namespace sigma
{
void connection::disconnect()
{
    auto d = m_disconnector.lock();

    if (d)
    {
        // attempt disconnect
        (*d)(*this);
    }
}
} // namespace sigma