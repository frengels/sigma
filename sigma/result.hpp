#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace sigma
{
template<typename T>
class Result final {
public:
    using value_type = T;

private:
    bool m_exceptional{false};

    union
    {
        T                  m_ok;
        std::exception_ptr m_error;
    };

public:
    /**
     * constructs a result from a type U if T can be constructed from U. If U
     * can not be converted to T then this constructor must be explicitly called
     */
    template<
        typename U,
        std::enable_if_t<
            std::is_constructible_v<T, U&&> && std::is_convertible_v<U&&, T> &&
                !std::is_same_v<std::decay_t<U>, std::in_place_t> &&
                !std::is_same_v<std::decay_t<U>, std::exception_ptr>,
            int> = 0>
    constexpr Result(U&& val) : m_ok{std::forward<U>(val)}
    {}

    template<
        typename U,
        std::enable_if_t<
            std::is_constructible_v<T, U&&> && !std::is_convertible_v<U&&, T> &&
                !std::is_same_v<std::decay_t<U>, std::in_place_t> &&
                !std::is_same_v<std::decay_t<U>, std::exception_ptr>,
            int> = 0>
    constexpr explicit Result(U&& val) : m_ok{std::forward<U>(val)}
    {}

    template<typename... Args,
             std::enable_if_t<std::is_constructible_v<T, Args...>, int> = 0>
    constexpr Result(std::in_place_t, Args&&... args)
        : m_ok{std::forward<Args>(args)...}
    {}

    /**
     * constructs a result from an exception when the result is invalid
     */
    constexpr Result(const std::exception_ptr& e)
        : m_exceptional{true}, m_error{e}
    {}

    ~Result()
    {
        if (m_exceptional)
        {
            m_error.~exception_ptr();
        }
        else
        {
            m_ok.~T();
        }
    }

    constexpr bool has_value() const noexcept
    {
        return !m_exceptional;
    }

    inline constexpr operator bool() const noexcept
    {
        return has_value();
    }

    constexpr T* operator->() noexcept(false)
    {
        if (m_exceptional)
        {
            rethrow();
        }

        return std::addressof(m_ok);
    }

    constexpr const T* operator->() const noexcept(false)
    {
        if (m_exceptional)
        {
            rethrow();
        }
        return std::addressof(m_ok);
    }

    template<typename U>
    constexpr T value_or(U&& default_val) &&
    {
        return has_value() ? std::move(this->m_ok) :
                             static_cast<T>(std::forward<U>(default_val));
    }

    template<typename U>
    constexpr T value_or(U&& default_val) const&
    {
        return has_value() ? this->m_ok :
                             static_cast<T>(std::forward<U>(default_val));
    }

    constexpr T& value() &
    {
        if (m_exceptional)
        {
            rethrow();
        }

        return this->m_ok;
    }

    constexpr const T& value() const&
    {
        if (m_exceptional)
        {
            rethrow();
        }

        return this->m_ok;
    }

    constexpr T&& value() &&
    {
        if (m_exceptional)
        {
            rethrow();
        }

        return std::move(this->m_ok);
    }

    constexpr const T&& value() const&&
    {
        if (m_exceptional)
        {
            rethrow();
        }

        return std::move(this->m_ok);
    }

    void try_rethrow()
    {
        if (m_exceptional)
        {
            rethrow();
        }
    }

private:
    [[noreturn]] void rethrow() { std::rethrow_exception(m_error); }
};
} // namespace sigma
