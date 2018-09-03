#pragma once

#include <memory>

namespace sigma
{
class connection;

class disconnector_base : std::enable_shared_from_this<disconnector_base> {
protected:
    disconnector_base() = default;

public:
    std::weak_ptr<disconnector_base> track() noexcept
    {
        return weak_from_this();
    }

    virtual void operator()(connection& c) = 0;
};
} // namespace sigma