#pragma once

#include <memory>

namespace sigma
{
template<typename H>
class connection;

template<typename H>
class disconnector_base : std::enable_shared_from_this<disconnector_base<H>> {
public:
    using handle_type     = H;
    using connection_type = sigma::connection<handle_type>;

protected:
    disconnector_base() = default;

public:
    std::weak_ptr<disconnector_base<handle_type>> track() noexcept
    {
        return this->weak_from_this();
    }

    virtual bool is_alive(const connection<handle_type>& c) const noexcept = 0;
    virtual void operator()(connection<handle_type>& c)                    = 0;
};
} // namespace sigma