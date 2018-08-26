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

    void*         m_instance{nullptr};
    callback_type m_callback{&FunctionRef::default_uninit};

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
             std::enable_if_t<std::is_invocable_v<Callable, Args...>, int> = 0>
    constexpr explicit FunctionRef(Callable& callable) noexcept
        : m_instance{std::addressof(callable)},
          m_callback([](void* obj, Args... args) -> return_type {
              std::decay_t<Callable>& callable =
                  *reinterpret_cast<Callable*>(obj);
              return std::invoke(callable, std::forward<Args>(args)...);
          })
    {}
    /*
        template<
            typename Ret_,
            typename... Args_,
            std::enable_if<sigma::is_compatible_function<return_type(Args...),
                                                         Ret_(Args_...)>::value,
                           int>::type = 0>
        constexpr explicit FunctionRef(Ret_ (*func)(Args_...))
            : m_instance{reinterpret_cast<void*>(func)},
              m_callback([](void* f, Args... args) -> return_type {
                  using fn_ptr_type = Ret_ (*)(Args_);
                  auto fn_ptr       = reinterpret_cast<fn_ptr_type>(f);
                  return static_cast<return_type>(
                      std::invoke(fn_ptr, static_cast<Args_>(args)...));
              })
        {}
        */

    constexpr inline return_type operator()(Args... args) const
        noexcept(noexcept(
            std::invoke(m_callback, m_instance, std::forward<Args>(args)...)))
    {
        return std::invoke(m_callback, m_instance, std::forward<Args>(args)...);
    }

    constexpr operator bool() const noexcept;

    template<auto MemFn, typename T>
    static constexpr std::enable_if_t<std::is_class_v<T>, FunctionRef>
    member_fn(T* obj)
    {
        static_assert(std::is_member_function_pointer_v<decltype(MemFn)>,
                      "Template argument must be a member function");
        static_assert(std::is_base_of_v<
                          std::decay_t<typename sigma::get_class_from_mem_fn<
                              decltype(MemFn)>::type>,
                          std::decay_t<T>>,
                      "Member function class is not a base of T");
        static_assert(std::is_invocable_v<decltype(MemFn), T, Args...>,
                      "Member function is not invocable on object of type T");

        FunctionRef res;

        if constexpr (std::is_const_v<T>)
        {
            res.m_instance = const_cast<std::remove_const_t<T>*>(obj);
        }
        else
        {
            res.m_instance = obj;
        }

        res.m_callback = [](void* obj, Args... args) -> return_type {
            auto tobj         = static_cast<T*>(obj);
            using mem_fn_type = decltype(MemFn);
            auto mem_fn       = MemFn;
            return std::invoke(mem_fn, tobj, std::forward<Args>(args)...);
        };

        return res;
    }

    template<auto MemFn, typename T>
    static constexpr std::enable_if_t<std::is_class_v<T>, FunctionRef>
    member_fn(T& obj)
    {
        return member_fn<MemFn>(std::addressof(obj));
    }
};

} // namespace sigma