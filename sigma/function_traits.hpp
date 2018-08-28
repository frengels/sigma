#pragma once

#include <functional>
#include <tuple>

namespace sigma
{
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

// generic false_type
template<typename Signature>
struct IsFunction : public std::false_type
{};

// normal functions
template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...)> : public std::true_type
{};

// variadic
template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......)> : public std::true_type
{};

// cv qualifiers
template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const volatile> : public std::true_type
{};

// ref qualifiers

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...)&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......)&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) &&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) &&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const volatile&&> : public std::true_type
{};

// noexcept qualifiers

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) volatile noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) volatile& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) volatile&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args......) const volatile&& noexcept>
    : public std::true_type
{};

template<typename Signature>
struct IsFunction<Signature(*)> : public sigma::IsFunction<Signature>
{};

template<typename Signature>
struct IsFunction<Signature(&)> : public sigma::IsFunction<Signature>
{};

template<typename T, typename Signature>
struct IsFunction<Signature(T::*)> : public sigma::IsFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsFunctionV = IsFunction<Signature>::value;

// variadic

template<typename Signature>
struct IsVariadicFunction : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......)> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const volatile>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......)&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const volatile&>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) &&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const volatile&&>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVariadicFunction<Ret(Args......) const volatile&& noexcept>
    : public std::true_type
{};

// variadic helpers

template<typename Signature>
struct IsVariadicFunction<Signature(*)>
    : public sigma::IsVariadicFunction<Signature>
{};

template<typename Signature>
struct IsVariadicFunction<Signature(&)>
    : public sigma::IsVariadicFunction<Signature>
{};

template<typename T, typename Signature>
struct IsVariadicFunction<Signature(T::*)>
    : public sigma::IsVariadicFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsVariadicFunctionV =
    IsVariadicFunction<Signature>::value;

// const function

// default
template<typename Signature>
struct IsConstFunction : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args...) const volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsConstFunction<Ret(Args......) const volatile&& noexcept>
    : public std::true_type
{};

// helpers

template<typename Signature>
struct IsConstFunction<Signature(*)> : public sigma::IsConstFunction<Signature>
{};

template<typename Signature>
struct IsConstFunction<Signature(&)> : public sigma::IsConstFunction<Signature>
{};

template<typename T, typename Signature>
struct IsConstFunction<Signature(T::*)>
    : public sigma::IsConstFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsConstFunctionV = IsConstFunction<Signature>::value;

// volatile functions
template<typename Signature>
struct IsVolatileFunction : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) const volatile> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) const volatile>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) const volatile&>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) const volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) const volatile&&>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args...) const volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsVolatileFunction<Ret(Args......) const volatile&& noexcept>
    : public std::true_type
{};

// helpers

template<typename Signature>
struct IsVolatileFunction<Signature(*)>
    : public sigma::IsVolatileFunction<Signature>
{};

template<typename Signature>
struct IsVolatileFunction<Signature(&)>
    : public sigma::IsVolatileFunction<Signature>
{};

template<typename T, typename Signature>
struct IsVolatileFunction<Signature(T::*)>
    : public sigma::IsVolatileFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsVolatileFunctionV =
    IsVolatileFunction<Signature>::value;

// lvalue qualified

template<typename Signature>
struct IsLvalueFunction : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...)&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......)&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) const&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) const volatile&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) volatile& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args...) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsLvalueFunction<Ret(Args......) const volatile& noexcept>
    : public std::true_type
{};

// helpers

template<typename Signature>
struct IsLvalueFunction<Signature(*)>
    : public sigma::IsLvalueFunction<Signature>
{};

template<typename Signature>
struct IsLvalueFunction<Signature(&)>
    : public sigma::IsLvalueFunction<Signature>
{};

template<typename T, typename Signature>
struct IsLvalueFunction<Signature(T::*)>
    : public sigma::IsLvalueFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsLvalueFunctionV = IsLvalueFunction<Signature>::value;

// rvalue qualified functions

template<typename Signature>
struct IsRvalueFunction : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) &&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) &&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) const&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) const volatile&&> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) const volatile&&>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) const&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) const&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args...) const volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsRvalueFunction<Ret(Args......) const volatile&& noexcept>
    : public std::true_type
{};

// helpers

template<typename Signature>
struct IsRvalueFunction<Signature(*)>
    : public sigma::IsRvalueFunction<Signature>
{};

template<typename Signature>
struct IsRvalueFunction<Signature(&)>
    : public sigma::IsRvalueFunction<Signature>
{};

template<typename T, typename Signature>
struct IsRvalueFunction<Signature(T::*)>
    : public sigma::IsRvalueFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsRvalueFunctionV = IsRvalueFunction<Signature>::value;

// nothrow functions

template<typename Signature>
struct IsNothrowFunction : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) const noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) const noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) volatile noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) const volatile noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) & noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) const& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) const& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) const volatile& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) && noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) const&& noexcept> : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) const&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args...) const volatile&& noexcept>
    : public std::true_type
{};

template<typename Ret, typename... Args>
struct IsNothrowFunction<Ret(Args......) const volatile&& noexcept>
    : public std::true_type
{};

// helpers

template<typename Signature>
struct IsNothrowFunction<Signature(*)>
    : public sigma::IsNothrowFunction<Signature>
{};

template<typename Signature>
struct IsNothrowFunction<Signature(&)>
    : public sigma::IsNothrowFunction<Signature>
{};

template<typename T, typename Signature>
struct IsNothrowFunction<Signature(T::*)>
    : public sigma::IsNothrowFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsNothrowFunctionV = IsNothrowFunction<Signature>::value;

// member function

template<typename Signature>
struct IsMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsMemberFunctionPointer<Signature(T::*)>
    : public sigma::IsFunction<Signature>
{};

template<typename Signature>
inline constexpr bool IsMemberFunctionPointerV =
    IsMemberFunctionPointer<Signature>::value;

// member function pointer class

template<typename Signature>
struct MemberFunctionPointerClass;

template<typename T, typename Signature>
struct MemberFunctionPointerClass<Signature(T::*)>
{
    using type = T;
};

template<typename Signature>
using MemberFunctionPointerClassT =
    typename MemberFunctionPointerClass<Signature>::type;

template<typename Signature>
struct FunctionSignature;

template<typename Signature>
struct FunctionSignature<Signature(*)>
{
    static_assert(sigma::IsFunctionV<Signature>,
                  "Signature(*) is not actually a function?");
    using type = Signature;
};

template<typename Signature>
struct FunctionSignature<Signature(&)>
{
    static_assert(sigma::IsFunctionV<Signature>,
                  "Signature(&) is not actually a function?");
    using type = Signature;
};

template<typename T, typename Signature>
struct FunctionSignature<Signature(T::*)>
{
    static_assert(sigma::IsFunctionV<Signature>,
                  "Signature(T::*) is not actually a function?");
    using type = Signature;
};

template<typename Signature>
using FunctionSignatureT = typename sigma::FunctionSignature<Signature>::type;

// get nth element

template<std::size_t N, typename... Ts>
struct NthElement
{
    static_assert(N < sizeof...(Ts),
                  "provided N larger than the amount of types available");
    using type = typename std::tuple_element<N, std::tuple<Ts...>>::type;
};

template<std::size_t N, typename... Ts>
using NthElementT = typename NthElement<N, Ts...>::type;

// get nth parameter

template<std::size_t N, typename Signature>
struct NthParameter;

template<std::size_t N, typename Ret, typename... Args>
struct NthParameter<N, Ret(Args...)>
{
    static_assert(N < sizeof...(Args),
                  "provided N is bigger than the amount of parameters");
    using type = NthElementT<N, Args...>;
};

template<size_t N, typename Ret, typename... Args>
struct NthParameter<N, Ret (*)(Args...)>
    : public sigma::NthParameter<N, Ret(Args...)>
{};

template<std::size_t N, typename Ret, typename... Args>
struct NthParameter<N, Ret (&)(Args...)>
    : public sigma::NthParameter<N, Ret(Args...)>
{};

template<size_t N, typename T, typename Signature>
struct NthParameter<N, Signature(T::*)>
    : public sigma::NthParameter<N, Signature>
{};

template<size_t N, typename Signature>
using NthParameterT = typename NthParameter<N, Signature>::type;

// get parameters as tuple

template<typename Signature>
struct ParametersAsTuple;

template<typename Ret, typename... Args>
struct ParametersAsTuple<Ret(Args...)>
{
    using type = std::tuple<Args...>;
};

template<typename Ret, typename... Args>
struct ParametersAsTuple<Ret (*)(Args...)>
    : public sigma::ParametersAsTuple<Ret(Args...)>
{};

template<typename Signature>
using ParametersAsTupleT = typename ParametersAsTuple<Signature>::type;

// selecting overloads

template<typename Signature>
struct Overload
{
    using function_ptr_type = Signature(*);

    function_ptr_type value;

    constexpr Overload(Signature(*f)) : value{f}
    {}
    constexpr Overload(Signature(&f)) : value{f}
    {}
    constexpr Overload(Signature f) : value{f}
    {}
};

} // namespace sigma