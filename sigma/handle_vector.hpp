#pragma once

#include <algorithm>
#include <cstdint>
#include <queue>
#include <vector>

#include <boost/container/small_vector.hpp>

namespace sigma
{

namespace detail
{
template<typename T>
using small_vector_adapter = boost::container::small_vector<T, 8>;
}

struct bad_handle_access : public std::logic_error
{
    bad_handle_access() : std::logic_error{"trying to access a deleted object"}
    {}
};

template<typename I, typename V>
struct handle_base
{
    using id_type      = I;
    using version_type = V;

    id_type      id;
    version_type version;

    constexpr handle_base(id_type id, version_type version = 0)
        : id{id}, version{version}
    {}

    constexpr handle_base(const handle_base&) noexcept = default;
    constexpr handle_base& operator=(const handle_base&) noexcept = default;

    constexpr bool operator==(const handle_base& other) const noexcept
    {
        return id == other.id && version == other.version;
    }

    constexpr bool operator!=(const handle_base& other) const noexcept
    {
        return !(*this == other);
    }

    constexpr bool operator<(const handle_base& other) const noexcept
    {
        // handles with same id and different version should never be compared
        // and is ub
        return id < other.id;
    }

    constexpr bool operator<=(const handle_base& other) const noexcept
    {
        return id <= other.id;
    }

    constexpr bool operator>(const handle_base& other) const noexcept
    {
        return id > other.id;
    }

    constexpr bool operator>=(const handle_base& other) const noexcept
    {
        return id >= other.id;
    }
};

struct vector_handle : public handle_base<uint32_t, uint32_t>
{
    using handle_base::handle_base;
};

/**
 * this vector uses handles to uniquely identify objects so that elements
 * can internally move around without outside objects losing their reference
 */
template<typename T,
         template<typename> typename Vector = detail::small_vector_adapter>
class handle_vector {
public:
    using value_vector    = Vector<T>;
    using size_type       = typename value_vector::size_type;
    using value_type      = typename value_vector::value_type;
    using reference       = typename value_vector::reference;
    using const_reference = typename value_vector::const_reference;
    using iterator        = typename value_vector::iterator;
    using const_iterator  = typename value_vector::const_iterator;

    using handle_type   = vector_handle;
    using back_ref_list = Vector<handle_type::id_type>;
    using handle_list   = Vector<handle_type>;
    using free_list     = std::priority_queue<handle_type>;

private:
    // stores the actual objects
    value_vector m_values;

    back_ref_list m_back_ref;

    // stores references to actual object
    handle_list m_handles;
    // stores freed handles
    free_list m_free;

public:
    bool is_valid_handle(const handle_type& handle) const noexcept
    {
        return m_handles[static_cast<size_type>(handle.id)].version ==
               handle.version;
    }

    iterator begin() noexcept
    {
        return m_values.begin();
    }

    iterator end() noexcept
    {
        return m_values.end();
    }

    reference operator[](const handle_type& handle)
    {
        const auto& internal_handle = get_internal_handle(handle);
        return m_values[static_cast<size_type>(internal_handle.id)];
    }

    const_reference operator[](const handle_type& handle) const
    {
        const auto& internal_handle = get_internal_handle(handle);
        return m_values[static_cast<size_type>(internal_handle.id)];
    }

    template<typename... Args>
    handle_type emplace_back(Args&&... args) noexcept(
        std::is_nothrow_constructible_v<value_type, Args&&...>)
    {
        static_assert(std::is_constructible_v<value_type, Args&&...>,
                      "Cannot construct value_type from Args&&...");

        auto internal_id = std::size(m_values);
        m_values.emplace_back(std::forward<Args>(args)...);
        auto external_handle = next_handle(internal_id);
        // store our handle referencing back to the handle list
        m_back_ref.push_back(external_handle.id);
        return external_handle;
    }

    void erase(const handle_type& handle) noexcept(false)
    {
        auto& internal_handle = get_internal_handle(handle);
        auto  distance        = internal_handle.id;

        // erase our element
        auto it = m_values.begin() + distance;
        m_values.erase(it);

        // correct back references
        auto ref_it = m_back_ref.begin() + distance;
        m_back_ref.erase(ref_it);

        // every value after this was shifted to the left so correct
        ref_it = m_back_ref.begin() + distance;
        std::for_each(ref_it,
                      m_back_ref.end(),
                      [&](typename handle_type::id_type external_id) {
                          --m_handles[external_id].id;
                      });

        // increment version so subsequent accesses throw
        ++internal_handle.version;
    }

    size_type size() const noexcept
    {
        return m_values.size();
    }

    void reserve(size_type new_capacity)
    {
        m_values.reserve(new_capacity);
        m_back_ref.reserve(new_capacity);
        m_handles.reserve(new_capacity);
    }

private:
    handle_type next_handle(handle_type::id_type internal_id)
    {
        if (std::empty(m_free))
        {
            // create a new handle
            auto external_id = std::size(m_handles);
            // put handle to our internal value
            m_handles.emplace_back(internal_id);
            // return external handle to be used
            return handle_type{static_cast<handle_type::id_type>(external_id)};
        }

        auto handle = m_free.top();
        m_free.pop();
        // no need to increment version since that was already incremented on
        // removal

        // put our internal reference in place
        m_handles[handle.id].id = internal_id;

        return handle;
    }

    handle_type& get_internal_handle(const handle_type& handle) noexcept(false)
    {
        if (!is_valid_handle(handle))
        {
            throw bad_handle_access();
        }

        return m_handles[static_cast<size_type>(handle.id)];
    }

    const handle_type& get_internal_handle(const handle_type& handle) const
        noexcept(false)
    {
        if (!is_valid_handle(handle))
        {
            throw bad_handle_access();
        }

        return m_handles[static_cast<size_type>(handle.id)];
    }
};
} // namespace sigma