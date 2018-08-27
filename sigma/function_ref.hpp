#pragma once

#include <functional>
#include <type_traits>

#include "sigma/function_traits.hpp"

namespace sigma
{
template<typename Signature>
class FunctionRef;

template<typename Ret, typename... Args>
class FunctionRef<Ret(Args...)> final {
public:
    using return_type   = Ret;
    using signature     = return_type(Args...);
    using callback_type = return_type (*)(void*, Args...);

private:
    static return_type default_uninit(void*, Args...)
    {
        throw std::bad_function_call{};
    }

    void*               m_instance{nullptr};
    const callback_type m_callback{FunctionRef::default_uninit};

private:
    /**
     * constructor for the member functions
     */
    constexpr FunctionRef(void* obj, callback_type cb) noexcept
        : m_instance{obj}, m_callback{cb}
    {}

public:
    constexpr FunctionRef() noexcept = default;
    constexpr FunctionRef(std::nullptr_t) noexcept : FunctionRef{}
    {}
    constexpr FunctionRef(return_type (*f)(Args...)) noexcept
        : m_instance{reinterpret_cast<void*>(f)},
          m_callback([](void* f, Args... args) -> return_type {
              using function_ptr_type = return_type (*)(Args...);
              auto func               = reinterpret_cast<function_ptr_type>(f);
              return std::invoke(func, std::forward<Args>(args)...);
          })
    {}

    constexpr FunctionRef(const FunctionRef&) noexcept = default;
    constexpr FunctionRef& operator=(const FunctionRef&) noexcept = default;

    template<typename Callable,
             std::enable_if_t<std::is_invocable_v<Callable, Args...> &&
                                  !std::is_function_v<Callable>,
                              int> = 0>
    constexpr explicit FunctionRef(Callable& callable) noexcept
        : m_instance{const_cast<std::remove_const_t<Callable>*>(
              std::addressof(callable))},
          m_callback([](void* obj, Args... args) -> return_type {
              Callable& callable = *reinterpret_cast<Callable*>(obj);
              return std::invoke(callable, std::forward<Args>(args)...);
          })
    {}

    /**
     * invokes the stored function, operator() is never actually noexcept until
     * function_traits are implemented.
     */
    constexpr return_type operator()(Args... args) const noexcept(noexcept(
        std::invoke(m_callback, m_instance, std::forward<Args>(args)...)))
    {
        return std::invoke(m_callback, m_instance, std::forward<Args>(args)...);
    }

    constexpr operator bool() const noexcept
    {
        return m_callback != default_uninit;
    }

    /**
     * create a function reference to a member function
     * the function reference can be created as follows
     * sigma::FunctionRef<Signature>::member_fn<&class::mem_fn>(instance);
     */
    template<auto MemFn, typename T>
    static constexpr std::enable_if_t<std::is_class_v<T>, FunctionRef>
    member_fn(T& obj)
    {
        static_assert(std::is_member_function_pointer_v<decltype(MemFn)>,
                      "Template argument must be a member function");
        static_assert(
            std::is_base_of_v<
                typename sigma::get_class_from_mem_fn<decltype(MemFn)>::type,
                std::remove_reference_t<T>>,
            "Member function class is not a base of T");
        static_assert(std::is_invocable_v<decltype(MemFn), T, Args...>,
                      "Member function is not invocable on object of type T");

        return FunctionRef{
            static_cast<void*>(
                const_cast<std::remove_const_t<T>*>(std::addressof(obj))),
            [](void* obj, Args... args) -> return_type {
                T&   instance = *static_cast<T*>(obj);
                auto mem_fn   = MemFn;
                return std::invoke(
                    mem_fn, instance, std::forward<Args>(args)...);
            }};
    }
}; // namespace sigma

} // namespace sigma