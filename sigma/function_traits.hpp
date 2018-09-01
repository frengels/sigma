#ifndef SIGMA_FUNCTION_TRAITS_HPP_INCLUDED
#define SIGMA_FUNCTION_TRAITS_HPP_INCLUDED

#pragma once

#include <functional>
#include <tuple>

#include "sigma/signature_traits.hpp"

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

template<std::size_t N, typename Fn>
struct function_nth_parameter
    : public sigma::signature_nth_parameter<N, function_signature_t<Fn>>
{
    static_assert(sigma::is_function_v<Fn>, "Fn is not a function");
};

template<std::size_t N, typename Fn>
using function_nth_parameter_t = typename function_nth_parameter<N, Fn>::type;

template<std::size_t N, typename MemFn>
struct member_function_pointer_nth_parameter
    : public signature_nth_parameter<N,
                                     member_function_pointer_signature_t<MemFn>>
{
    static_assert(sigma::is_member_function_pointer_v<MemFn>,
                  "MemFn is not a member function pointer");
};

template<std::size_t N, typename MemFn>
using member_function_pointer_nth_parameter_t =
    typename member_function_pointer_nth_parameter<N, MemFn>::type;

// get parameters as tuple

template<typename Fn>
struct function_parameter_list
    : public signature_parameter_list<sigma::function_signature_t<Fn>>
{
    static_assert(sigma::is_function_v<Fn>, "Fn is not a function");
};

template<typename Fn>
using function_parameter_list_t = typename function_parameter_list<Fn>::type;

template<typename MemFn>
struct member_function_pointer_parameter_list
    : public signature_parameter_list<
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