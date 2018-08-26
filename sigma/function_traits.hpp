#pragma once

#include <functional>
#include <tuple>

namespace sigma
{
template<typename...>
struct FunctionTraits;

template<typename R, typename... Args>
struct FunctionTraits<R(Args...)>
{
    using return_type            = R;
    static constexpr size_t size = sizeof...(Args);

    template<size_t Num>
    struct Arg
    {
        using type =
            typename std::tuple_element<Num, std::tuple<Args...>>::type;
    };
};

template<typename R, typename... Args>
struct FunctionTraits<R (*)(Args...)> : public FunctionTraits<R(Args...)>
{};

template<typename R, typename... Args>
struct FunctionTraits<std::function<R(Args...)>>
    : public FunctionTraits<R(Args...)>
{};

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
              std::is_convertible<
                  std::tuple_element<ArgN, std::tuple<From...>>,
                  std::tuple_element<ArgN, std::tuple<To...>>>::value &&
                  _is_convertible<ArgN + 1,
                                  std::tuple<From...>,
                                  std::tuple<To...>>::value>
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

template<typename Signature>
struct get_class_from_mem_fn;

template<typename T, typename Ret, typename... Args>
struct get_class_from_mem_fn<Ret (T::*)(Args...)>
{
    using type = T;
};

template<typename T, typename Ret, typename... Args>
struct get_class_from_mem_fn<Ret (T::*)(Args...) const>
{
    using type = T;
};

template<typename Signature>
struct is_const_member_function_pointer : public std::false_type
{};

template<typename T, typename Ret, typename... Args>
struct is_const_member_function_pointer<Ret (T::*)(Args...) const>
    : public std::true_type
{};

} // namespace sigma