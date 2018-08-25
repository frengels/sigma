#pragma once

#include <stdexcept>
#include <string>

namespace sigma
{
class BadResultAccess : public std::logic_error {
public:
    BadResultAccess() : std::logic_error{"bad result access"}
    {}
};

template<typename T>
class Result final {
public:
    using value_type = T;

private:
    bool m_exceptional{false};

    union
    {
        T           m_ok;
        std::string m_error;
    };

public:
    /**
     * constructs a result from a type U if T can be constructed from U. If U
     * can not be converted to T then this constructor must be explicitly called
     */
    template<typename U,
             typename std::enable_if<
                 std::is_constructible<T, U&&>::value &&
                     std::is_convertible<U&&, T>::value &&
                     !std::is_convertible<U&&, std::exception>::value,
                 int>::type = 0>
    constexpr Result(U&& val) : m_ok{std::forward<U>(val)}
    {}

    template<typename U,
             typename std::enable_if<
                 std::is_constructible<T, U&&>::value &&
                     !std::is_convertible<U&&, T>::value &&
                     !std::is_convertible<U&&, std::exception>::value,
                 int>::type = 0>
    constexpr explicit Result(U&& val) : m_ok{std::forward<U>(val)}
    {}

    template<typename... Args,
             typename std::enable_if<std::is_constructible<T, Args...>::value,
                                     int>::type = 0>
    constexpr Result(std::in_place_t, Args&&... args)
        : m_ok{std::forward<Args>(args)...}
    {}

    /**
     * constructs a result from an exception when the result is invalid
     */
    constexpr Result(const std::exception& e)
        : m_exceptional{true}, m_error{e.what()}
    {}

    ~Result()
    {
        if (m_exceptional)
        {
            m_error.~basic_string();
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

    constexpr T& operator*() & noexcept
    {
        return m_ok;
    }

    constexpr const T& operator*() const& noexcept
    {
        return m_ok;
    }

    constexpr T&& operator*() && noexcept
    {
        return std::move(m_ok);
    }

    constexpr const T&& operator*() const&& noexcept
    {
        return std::move(m_ok);
    }

    constexpr T* operator->() noexcept
    {
        return std::addressof(m_ok);
    }

    constexpr const T* operator->() const noexcept
    {
        return std::addressof(m_ok);
    }

    template<typename U>
    constexpr T value_or(U&& default_val) &&
    {
        return has_value() ? std::move(**this) :
                             static_cast<T>(std::forward<U>(default_val));
    }

    template<typename U>
    constexpr T value_or(U&& default_val) const&
    {
        return has_value() ? **this :
                             static_cast<T>(std::forward<U>(default_val));
    }

    constexpr T& value() &
    {
        if (m_exceptional)
        {
            throw BadResultAccess{};
        }

        return **this;
    }

    constexpr const T& value() const&
    {
        if (m_exceptional)
        {
            throw BadResultAccess{};
        }

        return **this;
    }

    constexpr T&& value() &&
    {
        if (m_exceptional)
        {
            throw BadResultAccess{};
        }

        return std::move(**this);
    }

    constexpr const T&& value() const&&
    {
        if (m_exceptional)
        {
            throw BadResultAccess{};
        }

        return std::move(**this);
    }

    constexpr std::string& error() &
    {
        return m_error;
    }

    constexpr const std::string& error() const&
    {
        return m_error;
    }

    constexpr std::string&& error() &&
    {
        return std::move(m_error);
    }

    constexpr const std::string&& error() const&&
    {
        return std::move(m_error);
    }
};
} // namespace sigma
