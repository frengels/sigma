#ifndef SIGMA_FUNCTION_TRAITS_HPP_INCLUDED
#define SIGMA_FUNCTION_TRAITS_HPP_INCLUDED

#pragma once

#include <functional>
#include <tuple>

namespace sigma
{
namespace detail
{
// these 2 structs are used for conditional inheritance in template
// specialization
struct empty
{};

template<typename T>
struct identity
{
    using type = T;
};
} // namespace detail

template<typename Returned, typename Required>
struct is_convertible_return
    : public std::integral_constant<
          bool,
          std::is_void<Required>::value ||
              std::is_convertible<Returned, Required>::value>
{};

template<size_t ArgN, typename From, typename To>
struct _is_convertible;

template<size_t ArgN, typename... From, typename... To>
struct _is_convertible<ArgN, std::tuple<From...>, std::tuple<To...>>
    : public std::integral_constant<
          bool,
          ((sizeof...(From) >= sizeof...(To)) && ArgN > sizeof...(To)) ||
              (std::is_convertible<
                   std::tuple_element<ArgN, std::tuple<From...>>,
                   std::tuple_element<ArgN, std::tuple<To...>>>::value &&
               _is_convertible<ArgN + 1,
                               std::tuple<From...>,
                               std::tuple<To...>>::value)>
{};

template<typename Returned, typename Required>
using is_convertible_return_v =
    typename is_convertible_return<Returned, Required>::value;

template<typename Signature, typename Callee>
struct is_compatible_function;

/*
template<typename Ret, typename Ret2, typename... Args, typename... Args2>
    struct is_compatible_function<Ret(Args...), Ret2(Args2...)>
    : public std::integral_constant < bool,
    sigma::is_convertible_return_v<Ret2, Ret>&&
        sigma::_is_convertible<0, std::tuple<Args...>, std::tuple<Args2...>>
{};
*/

// add function qualifiers

template<typename Signature>
struct add_signature_variadic
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...)>
{
    using type = Ret(Args..., ...);
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const>
{
    using type = Ret(Args..., ...) const;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) volatile>
{
    using type = Ret(Args..., ...) volatile;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const volatile>
{
    using type = Ret(Args..., ...) const volatile;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...)&>
{
    using type = Ret(Args..., ...) &;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const&>
{
    using type = Ret(Args..., ...) const&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) volatile&>
{
    using type = Ret(Args..., ...) volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const volatile&>
{
    using type = Ret(Args..., ...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) &&>
{
    using type = Ret(Args..., ...) &&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const&&>
{
    using type = Ret(Args..., ...) const&&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) volatile&&>
{
    using type = Ret(Args..., ...) volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const volatile&&>
{
    using type = Ret(Args..., ...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) noexcept>
{
    using type = Ret(Args..., ...) noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const noexcept>
{
    using type = Ret(Args..., ...) const noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args..., ...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args..., ...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) & noexcept>
{
    using type = Ret(Args..., ...) & noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const& noexcept>
{
    using type = Ret(Args..., ...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args..., ...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args..., ...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) && noexcept>
{
    using type = Ret(Args..., ...) && noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args..., ...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args..., ...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_variadic<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args..., ...) const volatile&& noexcept;
};

template<typename Signature>
using add_signature_variadic_t =
    typename add_signature_variadic<Signature>::type;

// add const qualifier

template<typename Signature>
struct add_signature_const
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...)>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...)>
{
    using type = Ret(Args..., ...) const;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) volatile>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) volatile>
{
    using type = Ret(Args..., ...) const volatile;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...)&>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...)&>
{
    using type = Ret(Args..., ...) const&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) volatile&>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) volatile&>
{
    using type = Ret(Args..., ...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) &&>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) &&>
{
    using type = Ret(Args..., ...) const&&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) volatile&&>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) volatile&&>
{
    using type = Ret(Args..., ...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) noexcept>
{
    using type = Ret(Args..., ...) const noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) volatile noexcept>
{
    using type = Ret(Args..., ...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) & noexcept>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) & noexcept>
{
    using type = Ret(Args..., ...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) volatile& noexcept>
{
    using type = Ret(Args..., ...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) && noexcept>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) && noexcept>
{
    using type = Ret(Args..., ...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) const volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_const<Ret(Args..., ...) volatile&& noexcept>
{
    using type = Ret(Args..., ...) const volatile&& noexcept;
};

template<typename Signature>
using add_signature_const_t = typename add_signature_const<Signature>::type;

// add volatile qualifier

template<typename Signature>
struct add_signature_volatile
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...)>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...)>
{
    using type = Ret(Args..., ...) volatile;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) const>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) const>
{
    using type = Ret(Args..., ...) const volatile;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...)&>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...)&>
{
    using type = Ret(Args..., ...) volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) const&>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) const&>
{
    using type = Ret(Args..., ...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) &&>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) &&>
{
    using type = Ret(Args..., ...) volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) const&&>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) const&&>
{
    using type = Ret(Args..., ...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) noexcept>
{
    using type = Ret(Args..., ...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) const noexcept>
{
    using type = Ret(Args..., ...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) & noexcept>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) & noexcept>
{
    using type = Ret(Args..., ...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) const& noexcept>
{
    using type = Ret(Args..., ...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) && noexcept>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) && noexcept>
{
    using type = Ret(Args..., ...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) const volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_volatile<Ret(Args..., ...) const&& noexcept>
{
    using type = Ret(Args..., ...) const volatile&& noexcept;
};

template<typename Signature>
using add_signature_volatile_t =
    typename add_signature_volatile<Signature>::type;

// add lvalue qualifier

template<typename Signature>
struct add_signature_lvalue
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...)>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...)>
{
    using type = Ret(Args..., ...) &;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) const>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) const>
{
    using type = Ret(Args..., ...) const&;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) volatile>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) volatile>
{
    using type = Ret(Args..., ...) volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) const volatile>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) const volatile>
{
    using type = Ret(Args..., ...) const volatile&;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) noexcept>
{
    using type = Ret(Args..., ...) & noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) const noexcept>
{
    using type = Ret(Args..., ...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) volatile noexcept>
{
    using type = Ret(Args..., ...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_lvalue<Ret(Args..., ...) const volatile noexcept>
{
    using type = Ret(Args..., ...) const volatile& noexcept;
};

template<typename Signature>
using add_signature_lvalue_t = typename add_signature_lvalue<Signature>::type;

// add rvalue qualifier

template<typename Signature>
struct add_signature_rvalue
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...)>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...)>
{
    using type = Ret(Args..., ...) &&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) const>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) const>
{
    using type = Ret(Args..., ...) const&&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) volatile>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) volatile>
{
    using type = Ret(Args..., ...) volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) const volatile>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) const volatile>
{
    using type = Ret(Args..., ...) const volatile&&;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) noexcept>
{
    using type = Ret(Args..., ...) && noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) const noexcept>
{
    using type = Ret(Args..., ...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) volatile noexcept>
{
    using type = Ret(Args..., ...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) const volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_rvalue<Ret(Args..., ...) const volatile noexcept>
{
    using type = Ret(Args..., ...) const volatile&& noexcept;
};

template<typename Signature>
using add_signature_rvalue_t = typename add_signature_rvalue<Signature>::type;

// add nothrow qualifiers

template<typename Signature>
struct add_signature_nothrow
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...)>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...)>
{
    using type = Ret(Args..., ...) noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) const>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) const>
{
    using type = Ret(Args..., ...) const noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) volatile>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) volatile>
{
    using type = Ret(Args..., ...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) const volatile>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) const volatile>
{
    using type = Ret(Args..., ...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...)&>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...)&>
{
    using type = Ret(Args..., ...) & noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) const&>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) const&>
{
    using type = Ret(Args..., ...) const& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) volatile&>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) volatile&>
{
    using type = Ret(Args..., ...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) const volatile&>
{
    using type = Ret(Args..., ...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) &&>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) &&>
{
    using type = Ret(Args..., ...) && noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) const&&>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) const&&>
{
    using type = Ret(Args..., ...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) volatile&&>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) volatile&&>
{
    using type = Ret(Args..., ...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) const volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct add_signature_nothrow<Ret(Args..., ...) const volatile&&>
{
    using type = Ret(Args..., ...) const volatile&& noexcept;
};

// specializations for noexcept(false) are not necessary in my opinion because
// remove_signature_nothrow already handles this functionality

template<typename Signature>
using add_signature_nothrow_t = typename add_signature_nothrow<Signature>::type;

// decay function qualifiers

template<typename Signature>
struct remove_signature_variadic
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...)>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) volatile>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const volatile>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...)&>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const&>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) volatile&>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const volatile&>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) &&>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const&&>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) volatile&&>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const volatile&&>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) volatile noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const volatile noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) & noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const& noexcept>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) volatile& noexcept>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const volatile& noexcept>
{
    using type = Ret(Args...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) && noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const&& noexcept>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) volatile&& noexcept>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_variadic<Ret(Args..., ...) const volatile&& noexcept>
{
    using type = Ret(Args...) const volatile&& noexcept;
};

template<typename Signature>
using remove_signature_variadic_t =
    typename remove_signature_variadic<Signature>::type;

// removing const from signatures

template<typename Signature>
struct remove_signature_const
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const>
{
    using type = Ret(Args..., ...);
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const volatile>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const volatile>
{
    using type = Ret(Args..., ...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const&>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const&>
{
    using type = Ret(Args..., ...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const volatile&>
{
    using type = Ret(Args..., ...) volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const&&>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const&&>
{
    using type = Ret(Args..., ...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const volatile&&>
{
    using type = Ret(Args..., ...) volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const noexcept>
{
    using type = Ret(Args..., ...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const volatile noexcept>
{
    using type = Ret(Args..., ...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const& noexcept>
{
    using type = Ret(Args..., ...) & noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const volatile& noexcept>
{
    using type = Ret(Args..., ...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const&& noexcept>
{
    using type = Ret(Args..., ...) && noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_const<Ret(Args..., ...) const volatile&& noexcept>
{
    using type = Ret(Args..., ...) volatile&& noexcept;
};

template<typename Signature>
using remove_signature_const_t =
    typename remove_signature_const<Signature>::type;

// remove volatile from signature

template<typename Signature>
struct remove_signature_volatile
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) volatile>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) volatile>
{
    using type = Ret(Args..., ...);
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) const volatile>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) const volatile>
{
    using type = Ret(Args..., ...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) volatile&>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) volatile&>
{
    using type = Ret(Args..., ...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) const volatile&>
{
    using type = Ret(Args..., ...) const&;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) volatile&&>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) volatile&&>
{
    using type = Ret(Args..., ...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) const volatile&&>
{
    using type = Ret(Args..., ...) const&&;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) volatile noexcept>
{
    using type = Ret(Args..., ...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) const volatile noexcept>
{
    using type = Ret(Args..., ...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) volatile& noexcept>
{
    using type = Ret(Args..., ...) & noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) const volatile& noexcept>
{
    using type = Ret(Args..., ...) const& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) volatile&& noexcept>
{
    using type = Ret(Args..., ...) && noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_volatile<Ret(Args..., ...) const volatile&& noexcept>
{
    using type = Ret(Args..., ...) const&& noexcept;
};

template<typename Signature>
using remove_signature_volatile_t =
    typename remove_signature_volatile<Signature>::type;

// remove lvalue qualifier from signature

template<typename Signature>
struct remove_signature_lvalue
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...)&>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...)&>
{
    using type = Ret(Args..., ...);
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) const&>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) const&>
{
    using type = Ret(Args..., ...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) volatile&>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) volatile&>
{
    using type = Ret(Args..., ...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) const volatile&>
{
    using type = Ret(Args..., ...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) & noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) & noexcept>
{
    using type = Ret(Args..., ...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) const& noexcept>
{
    using type = Ret(Args..., ...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) volatile& noexcept>
{
    using type = Ret(Args..., ...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_lvalue<Ret(Args..., ...) const volatile& noexcept>
{
    using type = Ret(Args..., ...) const volatile noexcept;
};

template<typename Signature>
using remove_signature_lvalue_t =
    typename remove_signature_lvalue<Signature>::type;

// remove rvalue qualifier from signature

template<typename Signature>
struct remove_signature_rvalue
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) &&>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) &&>
{
    using type = Ret(Args..., ...);
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) const&&>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) const&&>
{
    using type = Ret(Args..., ...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) volatile&&>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) volatile&&>
{
    using type = Ret(Args..., ...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) const volatile&&>
{
    using type = Ret(Args..., ...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) && noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) && noexcept>
{
    using type = Ret(Args..., ...) noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) const&& noexcept>
{
    using type = Ret(Args..., ...) const noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) volatile&& noexcept>
{
    using type = Ret(Args..., ...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct remove_signature_rvalue<Ret(Args..., ...) const volatile&& noexcept>
{
    using type = Ret(Args..., ...) const volatile noexcept;
};

template<typename Signature>
using remove_signature_rvalue_t =
    typename remove_signature_rvalue<Signature>::type;

// remove noexcept qualifier from signature

template<typename Signature>
struct remove_signature_nothrow
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) noexcept>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) noexcept>
{
    using type = Ret(Args..., ...);
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) const noexcept>
{
    using type = Ret(Args..., ...) const;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) volatile noexcept>
{
    using type = Ret(Args..., ...) volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) const volatile noexcept>
{
    using type = Ret(Args..., ...) const volatile;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) & noexcept>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) & noexcept>
{
    using type = Ret(Args..., ...) &;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) const& noexcept>
{
    using type = Ret(Args..., ...) const&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) volatile& noexcept>
{
    using type = Ret(Args..., ...) volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) const volatile& noexcept>
{
    using type = Ret(Args..., ...) const volatile&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) && noexcept>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) && noexcept>
{
    using type = Ret(Args..., ...) &&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) const&& noexcept>
{
    using type = Ret(Args..., ...) const&&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) volatile&& noexcept>
{
    using type = Ret(Args..., ...) volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct remove_signature_nothrow<Ret(Args..., ...) const volatile&& noexcept>
{
    using type = Ret(Args..., ...) const volatile&&;
};

template<typename Signature>
using remove_signature_nothrow_t =
    typename remove_signature_nothrow<Signature>::type;

// remove variadic qualifier from functions

template<typename T>
struct remove_function_variadic : public remove_signature_variadic<T>
{};

template<typename Signature>
struct remove_function_variadic<Signature(*)>
    : public remove_function_variadic<Signature>
{};

template<typename Signature>
struct remove_function_variadic<Signature(&)>
    : public remove_function_variadic<Signature>
{};

template<typename Signature>
using remove_function_variadic_t =
    typename remove_function_variadic<Signature>::type;

// remove variadic qualifier from member function pointers

template<typename T>
struct remove_member_function_pointer_variadic
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_variadic<Signature(T::*)>
    : public remove_signature_variadic<Signature>
{};

template<typename MemFn>
using remove_member_function_pointer_variadic_t =
    typename remove_member_function_pointer_variadic<MemFn>::type;

// remove const qualifier from functions

template<typename Signature>
struct remove_function_const : public remove_signature_const<Signature>
{};

template<typename Signature>
struct remove_function_const<Signature(*)>
    : public remove_signature_const<Signature>
{};

template<typename Signature>
struct remove_function_const<Signature(&)>
    : public remove_signature_const<Signature>
{};

template<typename Fn>
using remove_function_const_t = typename remove_function_const<Fn>::type;

// remove const qualifier from member function pointers

template<typename T>
struct remove_member_function_pointer_const
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_const<Signature(T::*)>
    : public remove_signature_const<Signature>
{};

template<typename MemFn>
using remove_member_function_pointer_const_t =
    typename remove_member_function_pointer_const<MemFn>::type;

// remove volatile qualifier from functions

template<typename Signature>
struct remove_function_volatile : public remove_signature_volatile<Signature>
{};

template<typename Signature>
struct remove_function_volatile<Signature(*)>
    : public remove_signature_volatile<Signature>
{};

template<typename Signature>
struct remove_function_volatile<Signature(&)>
    : public remove_signature_volatile<Signature>
{};

template<typename Fn>
using remove_function_volatile_t = typename remove_function_volatile<Fn>::type;

// remove volatile qualifier from member functions

template<typename T>
struct remove_member_function_pointer_volatile
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_volatile<Signature(T::*)>
    : public remove_signature_volatile<Signature>
{};

template<typename MemFn>
using remove_member_function_pointer_volatile_t =
    typename remove_member_function_pointer_volatile<MemFn>::type;

// remove lvalue qualifier from functions

template<typename Signature>
struct remove_function_lvalue : public remove_signature_lvalue<Signature>
{};

template<typename Signature>
struct remove_function_lvalue<Signature(*)>
    : public remove_signature_lvalue<Signature>
{};

template<typename Signature>
struct remove_function_lvalue<Signature(&)>
    : public remove_signature_lvalue<Signature>
{};

template<typename Fn>
using remove_function_lvalue_t = typename remove_function_lvalue<Fn>::type;

// remove lvalue qualifier from member function pointers

template<typename T>
struct remove_member_function_pointer_lvalue
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_lvalue<Signature(T::*)>
    : public remove_member_function_pointer_lvalue<Signature>
{};

template<typename MemFn>
using remove_member_function_pointer_lvalue_t =
    typename remove_member_function_pointer_lvalue<MemFn>::type;

// remove rvalue qualifier from functions

template<typename Signature>
struct remove_function_rvalue : public remove_signature_rvalue<Signature>
{};

template<typename Signature>
struct remove_function_rvalue<Signature(*)>
    : public remove_signature_rvalue<Signature>
{};

template<typename Signature>
struct remove_function_rvalue<Signature(&)>
    : public remove_signature_rvalue<Signature>
{};

template<typename Fn>
using remove_function_rvalue_t = typename remove_function_rvalue<Fn>::type;

// remove rvalue qualifer from member function pointers

template<typename T>
struct remove_member_function_pointer_rvalue
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_rvalue<Signature(T::*)>
    : public remove_signature_rvalue<Signature>
{};

template<typename MemFn>
using remove_member_function_pointer_rvalue_t =
    typename remove_member_function_pointer_rvalue<MemFn>::type;

// remove noexcept qualifier from functions

template<typename Signature>
struct remove_function_nothrow : public remove_signature_nothrow<Signature>
{};

template<typename Signature>
struct remove_function_nothrow<Signature(*)>
    : public remove_signature_nothrow<Signature>
{};

template<typename Signature>
struct remove_function_nothrow<Signature(&)>
    : public remove_signature_nothrow<Signature>
{};

template<typename Fn>
using remove_function_nothrow_t = typename remove_function_nothrow<Fn>::type;

// remove noexcept qualifier from member function pointers

template<typename T>
struct remove_member_function_pointer_nothrow
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_nothrow<Signature(T::*)>
    : public remove_signature_nothrow<Signature>
{};

template<typename MemFn>
using remove_member_function_pointer_nothrow_t =
    typename remove_member_function_pointer_nothrow<MemFn>::type;

// remove all qualifiers from a signature

template<typename Signature>
struct remove_signature_qualifiers
{
    using type = remove_signature_variadic_t<
        remove_signature_const_t<remove_signature_volatile_t<
            remove_signature_lvalue_t<remove_signature_rvalue_t<
                remove_signature_nothrow_t<Signature>>>>>>;
};

template<typename Signature>
using remove_signature_qualifiers_t =
    typename remove_signature_qualifiers<Signature>::type;

// remove all qualifiers from a function pointer

template<typename Signature>
struct remove_function_qualifiers
    : public remove_signature_qualifiers<Signature>
{};

template<typename Signature>
struct remove_function_qualifiers<Signature(*)>
    : public remove_signature_qualifiers<Signature>
{};

template<typename Signature>
struct remove_function_qualifiers<Signature(&)>
    : public remove_signature_qualifiers<Signature>
{};

template<typename Signature>
using remove_function_qualifiers_t =
    typename remove_function_qualifiers<Signature>::type;

// remove member function qualifiers

template<typename T>
struct remove_member_function_pointer_qualifiers
{
    using type = T;
};

template<typename T, typename Signature>
struct remove_member_function_pointer_qualifiers<Signature(T::*)>
    : public remove_signature_qualifiers<Signature>
{};

template<typename T>
using remove_member_function_pointer_qualifiers_t =
    typename remove_member_function_pointer_qualifiers<T>::type;

namespace detail
{
template<typename Signature>
struct is_signature_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_helper<Ret(Args...)> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature
    : public detail::is_signature_helper<remove_signature_variadic_t<
          remove_signature_const_t<remove_signature_volatile_t<
              remove_signature_lvalue_t<remove_signature_rvalue_t<
                  remove_signature_nothrow_t<Signature>>>>>>>
{};

template<typename Signature>
inline constexpr bool is_signature_v = is_signature<Signature>::value;

// generic false_type
template<typename Signature>
struct is_function_signature : public is_signature<Signature>
{};

template<typename Signature>
inline constexpr bool is_function_signature_v =
    is_function_signature<Signature>::value;

// is function

template<typename Signature>
struct is_function : public is_signature<Signature>
{};

template<typename Signature>
struct is_function<Signature(*)> : public is_signature<Signature>
{};

template<typename Signature>
struct is_function<Signature(&)> : public is_signature<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_v = is_function<Fn>::value;

// is member function pointer

template<typename T>
struct is_member_function_pointer : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer<Signature(T::*)>
    : public is_signature<Signature>
{
    static_assert(std::is_class_v<T>, "T is not a class");
};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_v =
    is_member_function_pointer<MemFn>::value;

// get function signature

template<typename Signature>
struct function_signature
    : public std::conditional_t<sigma::is_signature_v<Signature>,
                                detail::identity<Signature>,
                                detail::empty>
{};

template<typename Signature>
struct function_signature<Signature(*)>
    : public std::conditional_t<sigma::is_signature_v<Signature>,
                                detail::identity<Signature>,
                                detail::empty>
{};

template<typename Signature>
struct function_signature<Signature(&)>
    : public std::conditional_t<sigma::is_signature_v<Signature>,
                                detail::identity<Signature>,
                                detail::empty>
{};

template<typename Signature>
using function_signature_t =
    typename sigma::function_signature<Signature>::type;

// member function pointer signature

template<typename T>
struct member_function_pointer_signature;

template<typename T, typename Signature>
struct member_function_pointer_signature<Signature(T::*)>
    : public std::conditional_t<sigma::is_signature_v<Signature>,
                                detail::identity<Signature>,
                                detail::empty>
{};

template<typename MemFn>
using member_function_pointer_signature_t =
    typename member_function_pointer_signature<MemFn>::type;

// variadic

namespace detail
{
template<typename T>
struct is_signature_variadic_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_variadic_helper<Ret(Args..., ...)> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature_variadic
    : public std::conditional_t<
          detail::is_signature_variadic_helper<
              remove_signature_const_t<remove_signature_volatile_t<
                  remove_signature_lvalue_t<remove_signature_rvalue_t<
                      remove_signature_nothrow<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool is_signature_variadic_v =
    is_signature_variadic<Signature>::value;

// variadic helpers

template<typename Fn>
struct is_function_variadic : public is_signature_variadic<Fn>
{};

template<typename Signature>
struct is_function_variadic<Signature(*)>
    : public sigma::is_signature_variadic<Signature>
{};

template<typename Signature>
struct is_function_variadic<Signature(&)>
    : public sigma::is_signature_variadic<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_variadic_v = is_function_variadic<Fn>::value;

template<typename T>
struct is_member_function_pointer_variadic : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer_variadic<Signature(T::*)>
    : public sigma::is_signature_variadic<Signature>
{};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_variadic_v =
    is_member_function_pointer_variadic<MemFn>::value;

// const function

namespace detail
{
template<typename Signature>
struct is_signature_const_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_const_helper<Ret(Args...) const> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature_const
    : public std::conditional_t<
          detail::is_signature_const_helper<
              remove_signature_variadic_t<remove_signature_volatile_t<
                  remove_signature_lvalue_t<remove_signature_rvalue_t<
                      remove_signature_nothrow_t<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool is_signature_const_v =
    is_signature_const<Signature>::value;

template<typename Fn>
struct is_function_const : public is_signature_const<Fn>
{};

template<typename Signature>
struct is_function_const<Signature(*)> : public is_signature_const<Signature>
{};

template<typename Signature>
struct is_function_const<Signature(&)> : public is_signature_const<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_const_v = is_function_const<Fn>::value;

// check for const member function pointer

template<typename T>
struct is_member_function_pointer_const : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer_const<Signature(T::*)>
    : public is_signature_const<Signature>
{};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_const_v =
    is_member_function_pointer_const<MemFn>::value;

// volatile functions

namespace detail
{
template<typename Signature>
struct is_signature_volatile_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_volatile_helper<Ret(Args...) volatile>
    : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature_volatile
    : public std::conditional_t<
          detail::is_signature_volatile_helper<
              remove_signature_variadic_t<remove_signature_const_t<
                  remove_signature_lvalue_t<remove_signature_rvalue_t<
                      remove_signature_nothrow_t<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool is_signature_volatile_v =
    is_signature_volatile<Signature>::value;

template<typename Fn>
struct is_function_volatile : public is_signature_volatile<Fn>
{};

template<typename Signature>
struct is_function_volatile<Signature(*)>
    : public is_signature_volatile<Signature>
{};

template<typename Signature>
struct is_function_volatile<Signature(&)>
    : public is_signature_volatile<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_volatile_v = is_function_volatile<Fn>::value;

// check for volatile member function pointer

template<typename T>
struct is_member_function_pointer_volatile : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer_volatile<Signature(T::*)>
    : public is_signature_volatile<Signature>
{};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_volatile_v =
    is_member_function_pointer_volatile<MemFn>::value;

// lvalue qualified

namespace detail
{
template<typename Signature>
struct is_signature_lvalue_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_lvalue_helper<Ret(Args...)&> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature_lvalue
    : public std::conditional_t<
          detail::is_signature_lvalue_helper<
              remove_signature_variadic_t<remove_signature_const_t<
                  remove_signature_volatile_t<remove_signature_rvalue_t<
                      remove_signature_nothrow_t<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool is_signature_lvalue_v =
    is_signature_lvalue<Signature>::value;

template<typename Fn>
struct is_function_lvalue : public is_signature_lvalue<Fn>
{};

template<typename Signature>
struct is_function_lvalue<Signature(*)> : public is_signature_lvalue<Signature>
{};

template<typename Signature>
struct is_function_lvalue<Signature(&)> : public is_signature_lvalue<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_lvalue_v = is_function_lvalue<Fn>::value;

// check for lvalue member function pointer

template<typename T>
struct is_member_function_pointer_lvalue : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer_lvalue<Signature(T::*)>
    : public is_signature_lvalue<Signature>
{};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_lvalue_v =
    is_member_function_pointer_lvalue<MemFn>::value;

// rvalue qualified functions

namespace detail
{
template<typename Signature>
struct is_signature_rvalue_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_rvalue_helper<Ret(Args...) &&> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature_rvalue
    : public std::conditional_t<
          detail::is_signature_rvalue_helper<
              remove_signature_variadic_t<remove_signature_const_t<
                  remove_signature_volatile_t<remove_signature_lvalue_t<
                      remove_signature_nothrow_t<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool is_signature_rvalue_v =
    is_signature_rvalue<Signature>::value;

template<typename Fn>
struct is_function_rvalue : public is_signature_rvalue<Fn>
{};

template<typename Signature>
struct is_function_rvalue<Signature(*)> : public is_signature_rvalue<Signature>
{};

template<typename Signature>
struct is_function_rvalue<Signature(&)> : public is_signature_rvalue<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_rvalue_v = is_function_rvalue<Fn>::value;

// check for rvalue member function pointer

template<typename T>
struct is_member_function_pointer_rvalue : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer_rvalue<Signature(T::*)>
    : public is_signature_rvalue<Signature>
{};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_rvalue_v =
    is_member_function_pointer_rvalue<MemFn>::value;

// nothrow functions

namespace detail
{
template<typename Signature>
struct is_signature_nothrow_helper : public std::false_type
{};

template<typename Ret, typename... Args>
struct is_signature_nothrow_helper<Ret(Args...) noexcept>
    : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct is_signature_nothrow
    : public std::conditional_t<
          detail::is_signature_nothrow_helper<
              remove_signature_variadic_t<remove_signature_const_t<
                  remove_signature_volatile_t<remove_signature_lvalue_t<
                      remove_signature_rvalue_t<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool is_signature_nothrow_v =
    is_signature_nothrow<Signature>::value;

template<typename Fn>
struct is_function_nothrow : public is_signature_nothrow<Fn>
{};

template<typename Signature>
struct is_function_nothrow<Signature(*)>
    : public is_signature_nothrow<Signature>
{};

template<typename Signature>
struct is_function_nothrow<Signature(&)>
    : public is_signature_nothrow<Signature>
{};

template<typename Fn>
inline constexpr bool is_function_nothrow_v = is_function_nothrow<Fn>::value;

// check for nothrow member function pointer

template<typename T>
struct is_member_function_pointer_nothrow : public std::false_type
{};

template<typename T, typename Signature>
struct is_member_function_pointer_nothrow<Signature(T::*)>
    : public is_signature_nothrow<Signature>
{};

template<typename MemFn>
inline constexpr bool is_member_function_pointer_nothrow_v =
    is_member_function_pointer_nothrow<MemFn>::value;

// member function pointer class

template<typename Signature>
struct member_function_pointer_class;

template<typename T, typename Signature>
struct member_function_pointer_class<Signature(T::*)>
    : public std::conditional_t<is_signature_v<Signature>,
                                detail::identity<T>,
                                detail::empty>
{};

template<typename Signature>
using member_function_pointer_class_t =
    typename member_function_pointer_class<Signature>::type;

// get return type

namespace detail
{
template<typename Sig>
struct signature_return_helper;

template<typename Ret, typename... Args>
struct signature_return_helper<Ret(Args...)>
{
    using type = Ret;
};
} // namespace detail

template<typename Signature>
struct signature_return : public detail::signature_return_helper<
                              sigma::remove_signature_qualifiers_t<Signature>>
{};

template<typename Signature>
using signature_return_t = typename signature_return<Signature>::type;

template<typename Fn>
struct function_return : public signature_return<Fn>
{};

template<typename Signature>
struct function_return<Signature(*)> : public signature_return<Signature>
{};

template<typename Signature>
struct function_return<Signature(&)> : public signature_return<Signature>
{};

template<typename Fn>
using function_return_t = typename function_return<Fn>::type;

// get member function return type

template<typename T>
struct member_function_pointer_return;

template<typename T, typename Signature>
struct member_function_pointer_return<Signature(T::*)>
    : public signature_return<Signature>
{};

template<typename MemFn>
using member_function_pointer_return_t =
    typename member_function_pointer_return<MemFn>::type;

// get parameters as tuple

namespace detail
{
template<typename Signature>
struct signature_arguments_as_tuple_helper;

template<typename Ret, typename... Args>
struct signature_arguments_as_tuple_helper<Ret(Args...)>
{
    using type = std::tuple<Args...>;
};
} // namespace detail

template<typename Signature>
struct signature_arguments_as_tuple
    : public detail::signature_arguments_as_tuple_helper<
          remove_signature_qualifiers_t<Signature>>
{};

template<typename Signature>
using signature_arguments_as_tuple_t =
    typename signature_arguments_as_tuple<Signature>::type;

// merge function signature from 2 tuples

namespace detail
{
// functionality to merge any tuple and other elements into a tuple
template<typename... Ts>
struct merge_tuples
{
    using type = std::tuple<Ts...>;
};

template<>
struct merge_tuples<>
{
    using type = std::tuple<>;
};

template<typename T>
struct merge_tuples<T>
{
    using type = std::tuple<T>;
};

template<typename... Ts>
struct merge_tuples<std::tuple<Ts...>>
{
    using type = std::tuple<Ts...>;
};

template<typename... T1s, typename... T2s>
struct merge_tuples<std::tuple<T1s...>, std::tuple<T2s...>>
{
    using type = std::tuple<T1s..., T2s...>;
};

template<typename T1, typename T2>
struct merge_tuples<T1, T2>
{
    using type = std::tuple<T1, T2>;
};

template<typename T, typename... Ts>
struct merge_tuples<T, std::tuple<Ts...>>
{
    using type = std::tuple<T, Ts...>;
};

template<typename... Ts, typename T>
struct merge_tuples<std::tuple<Ts...>, T>
{
    using type = std::tuple<Ts..., T>;
};

template<typename... Ts, typename T, typename... Rest>
struct merge_tuples<std::tuple<Ts...>, T, Rest...>
{
    using temp = typename merge_tuples<Rest...>::type;
    using type = typename merge_tuples<std::tuple<Ts..., T>, temp>::type;
};

template<typename T, typename... Ts, typename... Rest>
struct merge_tuples<T, std::tuple<Ts...>, Rest...>
{
    using temp = typename merge_tuples<Rest...>::type;
    using type = typename merge_tuples<std::tuple<T, Ts...>, temp>::type;
};

template<typename... T1s, typename... T2s, typename... Rest>
struct merge_tuples<std::tuple<T1s...>, std::tuple<T2s...>, Rest...>
{
    using temp = typename merge_tuples<Rest...>::type;
    using type = typename merge_tuples<std::tuple<T1s..., T2s...>, temp>::type;
};

template<typename T1, typename T2, typename... Rest>
struct merge_tuples<T1, T2, Rest...>
{
    using temp = typename merge_tuples<Rest...>::type;
    using type = typename merge_tuples<std::tuple<T1, T2>, temp>::type;
};

// the final merge from return and a single tuple
template<typename Signature>
struct make_signature_helper;

template<typename Ret, typename... Args>
struct make_signature_helper<Ret(std::tuple<Args...>)>
{
    using type = Ret(Args...);
};
} // namespace detail

template<typename Signature>
struct make_signature;

template<typename Ret, typename... Args>
struct make_signature<Ret(Args...)>
{
    using type = typename detail::make_signature_helper<Ret(
        typename detail::merge_tuples<Args...>::type)>::type;
};

template<typename Signature>
using make_signature_t = typename make_signature<Signature>::type;

// get nth element

template<std::size_t N, typename... Ts>
struct nth_element
{
    static_assert(N < sizeof...(Ts),
                  "provided N larger than the amount of types available");
    using type = typename std::tuple_element<N, std::tuple<Ts...>>::type;
};

template<std::size_t N, typename... Ts>
using nth_element_t = typename nth_element<N, Ts...>::type;

// get nth parameter

namespace detail
{
template<std::size_t N, typename Signature>
struct signature_nth_argument_helper;

template<std::size_t N, typename Ret, typename... Args>
struct signature_nth_argument_helper<N, Ret(Args...)>
{
    static_assert(N < sizeof...(Args), "N larger than the number of arguments");
    using type = sigma::nth_element_t<N, Args...>;
};
} // namespace detail

template<std::size_t N, typename Signature>
struct signature_nth_argument : detail::signature_nth_argument_helper<
                                    N,
                                    remove_signature_qualifiers_t<Signature>>
{
    static_assert(sigma::is_signature_v<Signature>,
                  "provided Signature is not a valid function signature");
};

template<std::size_t N, typename Signature>
using signature_nth_argument_t =
    typename signature_nth_argument<N, Signature>::type;

template<std::size_t N, typename Fn>
struct function_nth_argument
    : public signature_nth_argument<N, function_signature_t<Fn>>
{
    static_assert(sigma::is_function_v<Fn>, "Fn is not a function");
};

template<std::size_t N, typename Fn>
using function_nth_argument_t = typename function_nth_argument<N, Fn>::type;

template<std::size_t N, typename MemFn>
struct member_function_pointer_nth_argument
    : public signature_nth_argument<N,
                                    member_function_pointer_signature_t<MemFn>>
{
    static_assert(sigma::is_member_function_pointer_v<MemFn>,
                  "MemFn is not a member function pointer");
};

// get parameters as tuple

template<typename Fn>
struct function_arguments_as_tuple
    : public signature_arguments_as_tuple<sigma::function_signature_t<Fn>>
{
    static_assert(sigma::is_function_v<Fn>, "Fn is not a function");
};

template<typename Fn>
using function_arguments_as_tuple_t =
    typename function_arguments_as_tuple<Fn>::type;

template<typename MemFn>
struct member_function_pointer_arguments_as_tuple
    : public signature_arguments_as_tuple<
          sigma::member_function_pointer_signature_t<MemFn>>
{
    static_assert(sigma::is_member_function_pointer_v<MemFn>,
                  "MemFn is not a member function pointer");
};

// selecting overloads

template<typename Signature>
constexpr auto overload(Signature(*fn_ptr)) noexcept
    -> std::enable_if_t<sigma::is_signature_v<Signature> &&
                            sigma::is_function_v<decltype(fn_ptr)> &&
                            !sigma::is_signature_const_v<Signature> &&
                            !sigma::is_signature_volatile_v<Signature> &&
                            !sigma::is_signature_lvalue_v<Signature> &&
                            !sigma::is_signature_rvalue_v<Signature>,
                        Signature(*)>
{
    return fn_ptr;
}

template<typename Signature>
constexpr auto overload(Signature(&fn_ref)) noexcept
    -> std::enable_if_t<sigma::is_signature_v<Signature> &&
                            sigma::is_function_v<decltype(fn_ref)> &&
                            !sigma::is_signature_const_v<Signature> &&
                            !sigma::is_signature_volatile_v<Signature> &&
                            !sigma::is_signature_lvalue_v<Signature> &&
                            !sigma::is_signature_rvalue_v<Signature>,
                        Signature(&)>
{
    return fn_ref;
}

template<typename Signature, typename T>
constexpr auto overload(Signature(T::*mem_fn)) noexcept -> std::enable_if_t<
    sigma::is_signature_v<Signature> &&
        sigma::is_member_function_pointer_v<decltype(mem_fn)>,
    Signature(T::*)>
{
    return mem_fn;
}

} // namespace sigma

#endif