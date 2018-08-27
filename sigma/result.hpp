#pragma once

#include <cassert>
#include <exception>
#include <stdexcept>
#include <string>

namespace sigma
{
namespace detail
{

template<typename T, bool Nothrow>
class ResultBase {
protected:
    bool m_exceptional{false};

    union
    {
        T                  m_ok;
        std::exception_ptr m_error;
    };

    ResultBase(T&& ok) : m_ok{std::forward<T>(ok)}
    {}
    template<typename... Args>
    ResultBase(Args&&... args) : m_ok{std::forward<Args>(args)...}
    {}
    ResultBase(const std::exception_ptr& except)
        : m_exceptional{true}, m_error{except}
    {}

    ~ResultBase()
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
};

template<typename T>
class ResultBase<T, true> {
protected:
    T m_ok;

    ResultBase(T&& ok) : m_ok{std::move(ok)}
    {}
    template<typename... Args>
    ResultBase(Args&&... args) : m_ok{std::forward<Args>(args)...}
    {}
    ResultBase(bool dummy)
    {}

    ~ResultBase() = default;
};
} // namespace detail

template<typename T, bool Nothrow = false>
class Result final : public detail::ResultBase<T, Nothrow> {
public:
    static constexpr bool is_nothrow = Nothrow;
    using value_type                 = T;

    using base_type = detail::ResultBase<T, is_nothrow>;

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
    constexpr Result(U&& val) : base_type{std::forward<U>(val)}
    {}

    template<
        typename U,
        std::enable_if_t<
            std::is_constructible_v<T, U&&> && !std::is_convertible_v<U&&, T> &&
                !std::is_same_v<std::decay_t<U>, std::in_place_t> &&
                !std::is_same_v<std::decay_t<U>, std::exception_ptr>,
            int> = 0>
    constexpr explicit Result(U&& val) : base_type{std::forward<U>(val)}
    {}

    template<typename... Args,
             std::enable_if_t<std::is_constructible_v<T, Args...>, int> = 0>
    constexpr Result(std::in_place_t, Args&&... args)
        : base_type{std::forward<Args>(args)...}
    {}

    /**
     * constructs a result from an exception when the result is invalid
     */
    constexpr Result(const std::exception_ptr& e) : base_type{e}
    {
        static_assert(
            !is_nothrow,
            "cannot initialize a nothrow result with an exception_ptr");
    }

    constexpr bool has_value() const noexcept
    {
        if constexpr (is_nothrow)
        {
            return true;
        }
        else
        {
            return !this->m_exceptional;
        }
    }

    constexpr bool has_exception() const noexcept
    {
        if constexpr (is_nothrow)
        {
            return false;
        }
        else
        {
            return !has_value();
        }
    }

    inline constexpr operator bool() const noexcept
    {
        return has_value();
    }

    constexpr T* operator->() noexcept(is_nothrow)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }

        return std::addressof(this->m_ok);
    }

    constexpr const T* operator->() const noexcept(is_nothrow)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }
        return std::addressof(this->m_ok);
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

    constexpr T& value() & noexcept(is_nothrow)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }

        return this->m_ok;
    }

    constexpr const T& value() const& noexcept(is_nothrow)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }

        return this->m_ok;
    }

    constexpr T&& value() && noexcept(is_nothrow)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }

        return std::move(this->m_ok);
    }

    constexpr const T&& value() const&& noexcept(is_nothrow)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }

        return std::move(this->m_ok);
    }

    void try_rethrow() noexcept(false)
    {
        if constexpr (!is_nothrow)
        {
            if (this->m_exceptional)
            {
                rethrow();
            }
        }
    }

private:
    [[noreturn]] void rethrow() noexcept(false)
    {
        static_assert(!is_nothrow, "Cannot rethrow a nothrow Result");
        assert(this->m_exceptional &&
               "Tried rethrowing when there was no exception_ptr present");
        std::rethrow_exception(this->m_error);
    }
};
} // namespace sigma
