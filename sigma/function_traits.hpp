#pragma once

#include <functional>
#include <tuple>

namespace sigma
{
namespace detail
{
// these 2 structs are used for conditional inheritance in template
// specialization
struct Empty
{};

template<typename T>
struct Identity
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

// decay function qualifiers

// TODO write remove qualifier functions for all qualifiers and simplify
// isconstfunction, etc. to use these qualifier removers so you're left with
// R(A...) qualifier

template<typename Signature>
struct RemoveSignatureVariadic
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......)>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) volatile>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const volatile>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......)&>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const&>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) volatile&>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const volatile&>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) &&>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const&&>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) volatile&&>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const volatile&&>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) volatile noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const volatile noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) & noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const& noexcept>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) volatile& noexcept>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const volatile& noexcept>
{
    using type = Ret(Args...) const volatile& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) && noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const&& noexcept>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) volatile&& noexcept>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVariadic<Ret(Args......) const volatile&& noexcept>
{
    using type = Ret(Args...) const volatile&& noexcept;
};

template<typename Signature>
using RemoveSignatureVariadicT =
    typename RemoveSignatureVariadic<Signature>::type;

// removing const from signatures

template<typename Signature>
struct RemoveSignatureConst
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const>
{
    using type = Ret(Args......);
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const volatile>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const volatile>
{
    using type = Ret(Args......) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const&>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const&>
{
    using type = Ret(Args......) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const volatile&>
{
    using type = Ret(Args......) volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const&&>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const&&>
{
    using type = Ret(Args......) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const volatile&&>
{
    using type = Ret(Args......) volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const noexcept>
{
    using type = Ret(Args......) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const volatile noexcept>
{
    using type = Ret(Args......) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const& noexcept>
{
    using type = Ret(Args......) & noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const volatile& noexcept>
{
    using type = Ret(Args......) volatile& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const&& noexcept>
{
    using type = Ret(Args......) && noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) volatile&& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureConst<Ret(Args......) const volatile&& noexcept>
{
    using type = Ret(Args......) volatile&& noexcept;
};

template<typename Signature>
using RemoveSignatureConstT = typename RemoveSignatureConst<Signature>::type;

// remove volatile from signature

template<typename Signature>
struct RemoveSignatureVolatile
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) volatile>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) volatile>
{
    using type = Ret(Args......);
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) const volatile>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) const volatile>
{
    using type = Ret(Args......) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) volatile&>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) volatile&>
{
    using type = Ret(Args......) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) const volatile&>
{
    using type = Ret(Args......) const&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) volatile&&>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) volatile&&>
{
    using type = Ret(Args......) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) const volatile&&>
{
    using type = Ret(Args......) const&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) volatile noexcept>
{
    using type = Ret(Args......) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) const volatile noexcept>
{
    using type = Ret(Args......) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) & noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) volatile& noexcept>
{
    using type = Ret(Args......) & noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) const& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) const volatile& noexcept>
{
    using type = Ret(Args......) const& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) && noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) volatile&& noexcept>
{
    using type = Ret(Args......) && noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) const&& noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureVolatile<Ret(Args......) const volatile&& noexcept>
{
    using type = Ret(Args......) const&& noexcept;
};

template<typename Signature>
using RemoveSignatureVolatileT =
    typename RemoveSignatureVolatile<Signature>::type;

// remove lvalue qualifier from signature

template<typename Signature>
struct RemoveSignatureLvalue
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...)&>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......)&>
{
    using type = Ret(Args......);
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) const&>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) const&>
{
    using type = Ret(Args......) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) volatile&>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) volatile&>
{
    using type = Ret(Args......) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) const volatile&>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) const volatile&>
{
    using type = Ret(Args......) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) & noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) & noexcept>
{
    using type = Ret(Args......) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) const& noexcept>
{
    using type = Ret(Args......) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) volatile& noexcept>
{
    using type = Ret(Args......) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureLvalue<Ret(Args......) const volatile& noexcept>
{
    using type = Ret(Args......) const volatile noexcept;
};

template<typename Signature>
using RemoveSignatureLvalueT = typename RemoveSignatureLvalue<Signature>::type;

// remove rvalue qualifier from signature

template<typename Signature>
struct RemoveSignatureRvalue
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) &&>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) &&>
{
    using type = Ret(Args......);
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) const&&>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) const&&>
{
    using type = Ret(Args......) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) volatile&&>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) volatile&&>
{
    using type = Ret(Args......) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) const volatile&&>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) const volatile&&>
{
    using type = Ret(Args......) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) && noexcept>
{
    using type = Ret(Args...) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) && noexcept>
{
    using type = Ret(Args......) noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) const&& noexcept>
{
    using type = Ret(Args......) const noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) volatile&& noexcept>
{
    using type = Ret(Args......) volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) const volatile noexcept;
};

template<typename Ret, typename... Args>
struct RemoveSignatureRvalue<Ret(Args......) const volatile&& noexcept>
{
    using type = Ret(Args......) const volatile noexcept;
};

template<typename Signature>
using RemoveSignatureRvalueT = typename RemoveSignatureRvalue<Signature>::type;

// remove noexcept qualifier from signature

template<typename Signature>
struct RemoveSignatureNothrow
{
    using type = Signature;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) noexcept>
{
    using type = Ret(Args...);
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) noexcept>
{
    using type = Ret(Args......);
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) const noexcept>
{
    using type = Ret(Args...) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) const noexcept>
{
    using type = Ret(Args......) const;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) volatile noexcept>
{
    using type = Ret(Args...) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) volatile noexcept>
{
    using type = Ret(Args......) volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) const volatile noexcept>
{
    using type = Ret(Args...) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) const volatile noexcept>
{
    using type = Ret(Args......) const volatile;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) & noexcept>
{
    using type = Ret(Args...) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) & noexcept>
{
    using type = Ret(Args......) &;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) const& noexcept>
{
    using type = Ret(Args...) const&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) const& noexcept>
{
    using type = Ret(Args......) const&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) volatile& noexcept>
{
    using type = Ret(Args...) volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) volatile& noexcept>
{
    using type = Ret(Args......) volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) const volatile& noexcept>
{
    using type = Ret(Args...) const volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) const volatile& noexcept>
{
    using type = Ret(Args......) const volatile&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) && noexcept>
{
    using type = Ret(Args...) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) && noexcept>
{
    using type = Ret(Args......) &&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) const&& noexcept>
{
    using type = Ret(Args...) const&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) const&& noexcept>
{
    using type = Ret(Args......) const&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) volatile&& noexcept>
{
    using type = Ret(Args...) volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) volatile&& noexcept>
{
    using type = Ret(Args......) volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args...) const volatile&& noexcept>
{
    using type = Ret(Args...) const volatile&&;
};

template<typename Ret, typename... Args>
struct RemoveSignatureNothrow<Ret(Args......) const volatile&& noexcept>
{
    using type = Ret(Args......) const volatile&&;
};

template<typename Signature>
using RemoveSignatureNothrowT =
    typename RemoveSignatureNothrow<Signature>::type;

// remove variadic qualifier from functions

template<typename T>
struct RemoveFunctionVariadic : public RemoveSignatureVariadic<T>
{};

template<typename Signature>
struct RemoveFunctionVariadic<Signature(*)>
    : public RemoveFunctionVariadic<Signature>
{};

template<typename Signature>
struct RemoveFunctionVariadic<Signature(&)>
    : public RemoveFunctionVariadic<Signature>
{};

template<typename Signature>
using RemoveFunctionVariadicT =
    typename RemoveFunctionVariadic<Signature>::type;

// remove variadic qualifier from member function pointers

template<typename T>
struct RemoveMemberFunctionPointerVariadic
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerVariadic<Signature(T::*)>
    : public RemoveSignatureVariadic<Signature>
{};

template<typename MemFn>
using RemoveMemberFunctionPointerVariadicT =
    typename RemoveMemberFunctionPointerVariadic<MemFn>::type;

// remove const qualifier from functions

template<typename Signature>
struct RemoveFunctionConst : public RemoveSignatureConst<Signature>
{};

template<typename Signature>
struct RemoveFunctionConst<Signature(*)>
    : public RemoveSignatureConst<Signature>
{};

template<typename Signature>
struct RemoveFunctionConst<Signature(&)>
    : public RemoveSignatureConst<Signature>
{};

template<typename Fn>
using RemoveFunctionConstT = typename RemoveFunctionConst<Fn>::type;

// remove const qualifier from member function pointers

template<typename T>
struct RemoveMemberFunctionPointerConst
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerConst<Signature(T::*)>
    : public RemoveSignatureConst<Signature>
{};

template<typename MemFn>
using RemoveMemberFunctionPointerConstT =
    typename RemoveMemberFunctionPointerConst<MemFn>::type;

// remove volatile qualifier from functions

template<typename Signature>
struct RemoveFunctionVolatile : public RemoveSignatureVolatile<Signature>
{};

template<typename Signature>
struct RemoveFunctionVolatile<Signature(*)>
    : public RemoveSignatureVolatile<Signature>
{};

template<typename Signature>
struct RemoveFunctionVolatile<Signature(&)>
    : public RemoveSignatureVolatile<Signature>
{};

template<typename Fn>
using RemoveFunctionVolatileT = typename RemoveFunctionVolatile<Fn>::type;

// remove volatile qualifier from member functions

template<typename T>
struct RemoveMemberFunctionPointerVolatile
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerVolatile<Signature(T::*)>
    : public RemoveSignatureVolatile<Signature>
{};

template<typename MemFn>
using RemoveMemberFunctionPointerVolatileT =
    typename RemoveMemberFunctionPointerVolatile<MemFn>::type;

// remove lvalue qualifier from functions

template<typename Signature>
struct RemoveFunctionLvalue : public RemoveSignatureLvalue<Signature>
{};

template<typename Signature>
struct RemoveFunctionLvalue<Signature(*)>
    : public RemoveSignatureLvalue<Signature>
{};

template<typename Signature>
struct RemoveFunctionLvalue<Signature(&)>
    : public RemoveSignatureLvalue<Signature>
{};

template<typename Fn>
using RemoveFunctionLvalueT = typename RemoveFunctionLvalue<Fn>::type;

// remove lvalue qualifier from member function pointers

template<typename T>
struct RemoveMemberFunctionPointerLvalue
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerLvalue<Signature(T::*)>
    : public RemoveMemberFunctionPointerLvalue<Signature>
{};

template<typename MemFn>
using RemoveMemberFunctionPointerLvalueT =
    typename RemoveMemberFunctionPointerLvalue<MemFn>::type;

// remove rvalue qualifier from functions

template<typename Signature>
struct RemoveFunctionRvalue : public RemoveSignatureRvalue<Signature>
{};

template<typename Signature>
struct RemoveFunctionRvalue<Signature(*)>
    : public RemoveSignatureRvalue<Signature>
{};

template<typename Signature>
struct RemoveFunctionRvalue<Signature(&)>
    : public RemoveSignatureRvalue<Signature>
{};

template<typename Fn>
using RemoveFunctionRvalueT = typename RemoveFunctionRvalue<Fn>::type;

// remove rvalue qualifer from member function pointers

template<typename T>
struct RemoveMemberFunctionPointerRvalue
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerRvalue<Signature(T::*)>
    : public RemoveSignatureRvalue<Signature>
{};

template<typename MemFn>
using RemoveMemberFunctionPointerRvalueT =
    typename RemoveMemberFunctionPointerRvalue<MemFn>::type;

// remove noexcept qualifier from functions

template<typename Signature>
struct RemoveFunctionNothrow : public RemoveSignatureNothrow<Signature>
{};

template<typename Signature>
struct RemoveFunctionNothrow<Signature(*)>
    : public RemoveSignatureNothrow<Signature>
{};

template<typename Signature>
struct RemoveFunctionNothrow<Signature(&)>
    : public RemoveSignatureNothrow<Signature>
{};

template<typename Fn>
using RemoveFunctionNothrowT = typename RemoveFunctionNothrow<Fn>::type;

// remove noexcept qualifier from member function pointers

template<typename T>
struct RemoveMemberFunctionPointerNothrow
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerNothrow<Signature(T::*)>
    : public RemoveSignatureNothrow<Signature>
{};

template<typename MemFn>
using RemoveMemberFunctionPointerNothrowT =
    typename RemoveMemberFunctionPointerNothrow<MemFn>::type;

// remove all qualifiers from a signature

template<typename Signature>
struct RemoveSignatureQualifiers
{
    using type = RemoveSignatureVariadicT<
        RemoveSignatureConstT<RemoveSignatureVolatileT<RemoveSignatureLvalueT<
            RemoveSignatureRvalue<RemoveSignatureNothrowT<Signature>>>>>>;
};

template<typename Signature>
using RemoveSignatureQualifiersT =
    typename RemoveSignatureQualifiers<Signature>::type;

// remove all qualifiers from a function pointer

template<typename Signature>
struct RemoveFunctionQualifiers : public RemoveSignatureQualifiers<Signature>
{};

template<typename Signature>
struct RemoveFunctionQualifiers<Signature(*)>
    : public RemoveSignatureQualifiers<Signature>
{};

template<typename Signature>
struct RemoveFunctionQualifiers<Signature(&)>
    : public RemoveSignatureQualifiers<Signature>
{};

template<typename Signature>
using RemoveFunctionQualifiersT =
    typename RemoveFunctionQualifiers<Signature>::type;

// remove member function qualifiers

template<typename T>
struct RemoveMemberFunctionPointerQualifiers
{
    using type = T;
};

template<typename T, typename Signature>
struct RemoveMemberFunctionPointerQualifiers<Signature(T::*)>
    : public RemoveSignatureQualifiers<Signature>
{};

template<typename T>
using RemoveMemberFunctionPointerQualifiersT =
    typename RemoveMemberFunctionPointerQualifiers<T>::type;

namespace detail
{
template<typename Signature>
struct IsSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsSignatureHelper<Ret(Args...)> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsSignature
    : public detail::IsSignatureHelper<RemoveSignatureVariadicT<
          RemoveSignatureConstT<RemoveSignatureVolatileT<RemoveSignatureLvalueT<
              RemoveSignatureRvalueT<RemoveSignatureNothrowT<Signature>>>>>>>
{};

template<typename Signature>
inline constexpr bool IsSignatureV = IsSignature<Signature>::value;

// generic false_type
template<typename Signature>
struct IsFunctionSignature : public IsSignature<Signature>
{};

template<typename Signature>
inline constexpr bool IsFunctionSignatureV =
    IsFunctionSignature<Signature>::value;

// is function

template<typename Signature>
struct IsFunction : public IsSignature<Signature>
{};

template<typename Signature>
struct IsFunction<Signature(*)> : public IsSignature<Signature>
{};

template<typename Signature>
struct IsFunction<Signature(&)> : public IsSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsFunctionV = IsFunction<Fn>::value;

// is member function pointer

template<typename T>
struct IsMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsMemberFunctionPointer<Signature(T::*)> : public IsSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsMemberFunctionPointerV =
    IsMemberFunctionPointer<MemFn>::value;

// get function signature

template<typename Signature>
struct FunctionSignature
    : public std::conditional_t<sigma::IsSignatureV<Signature>,
                                detail::Identity<Signature>,
                                detail::Empty>
{};

template<typename Signature>
struct FunctionSignature<Signature(*)>
    : public std::conditional_t<sigma::IsSignatureV<Signature>,
                                detail::Identity<Signature>,
                                detail::Empty>
{};

template<typename Signature>
struct FunctionSignature<Signature(&)>
    : public std::conditional_t<sigma::IsSignatureV<Signature>,
                                detail::Identity<Signature>,
                                detail::Empty>
{};

template<typename Signature>
using FunctionSignatureT = typename sigma::FunctionSignature<Signature>::type;

// member function pointer signature

template<typename T>
struct MemberFunctionPointerSignature;

template<typename T, typename Signature>
struct MemberFunctionPointerSignature<Signature(T::*)>
    : public std::conditional_t<sigma::IsSignatureV<Signature>,
                                detail::Identity<Signature>,
                                detail::Empty>
{};

template<typename MemFn>
using MemberFunctionPointerSignatureT =
    typename MemberFunctionPointerSignature<MemFn>::type;

// variadic

namespace detail
{
template<typename T>
struct IsVariadicSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsVariadicSignatureHelper<Ret(Args......)> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsVariadicSignature
    : public std::conditional_t<
          detail::IsVariadicSignatureHelper<
              RemoveSignatureConstT<RemoveSignatureVolatileT<
                  RemoveSignatureLvalueT<RemoveSignatureRvalueT<
                      RemoveSignatureNothrow<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool IsVariadicSignatureV =
    IsVariadicSignature<Signature>::value;

// variadic helpers

template<typename Fn>
struct IsVariadicFunction : public IsVariadicSignature<Fn>
{};

template<typename Signature>
struct IsVariadicFunction<Signature(*)>
    : public sigma::IsVariadicSignature<Signature>
{};

template<typename Signature>
struct IsVariadicFunction<Signature(&)>
    : public sigma::IsVariadicSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsVariadicFunctionV = IsVariadicFunction<Fn>::value;

template<typename T>
struct IsVariadicMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsVariadicMemberFunctionPointer<Signature(T::*)>
    : public sigma::IsVariadicSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsVariadicMemberFunctionPointerV =
    IsVariadicMemberFunctionPointer<MemFn>::value;

// const function

namespace detail
{
template<typename Signature>
struct IsConstSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsConstSignatureHelper<Ret(Args...) const> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsConstSignature
    : public std::conditional_t<
          detail::IsConstSignatureHelper<
              RemoveSignatureVariadicT<RemoveSignatureVolatileT<
                  RemoveSignatureLvalueT<RemoveSignatureRvalueT<
                      RemoveSignatureNothrowT<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool IsConstSignatureV = IsConstSignature<Signature>::value;

template<typename Fn>
struct IsConstFunction : public IsConstSignature<Fn>
{};

template<typename Signature>
struct IsConstFunction<Signature(*)> : public IsConstSignature<Signature>
{};

template<typename Signature>
struct IsConstFunction<Signature(&)> : public IsConstSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsConstFunctionV = IsConstFunction<Fn>::value;

// check for const member function pointer

template<typename T>
struct IsConstMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsConstMemberFunctionPointer<Signature(T::*)>
    : public IsConstSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsConstMemberFunctionPointerV =
    IsConstMemberFunctionPointer<MemFn>::value;

// volatile functions

namespace detail
{
template<typename Signature>
struct IsVolatileSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsVolatileSignatureHelper<Ret(Args...) volatile> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsVolatileSignature
    : public std::conditional_t<
          detail::IsVolatileSignatureHelper<
              RemoveSignatureVariadicT<RemoveSignatureConstT<
                  RemoveSignatureLvalueT<RemoveSignatureRvalueT<
                      RemoveSignatureNothrowT<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool IsVolatileSignatureV =
    IsVolatileSignature<Signature>::value;

template<typename Fn>
struct IsVolatileFunction : public IsVolatileSignature<Fn>
{};

template<typename Signature>
struct IsVolatileFunction<Signature(*)> : public IsVolatileSignature<Signature>
{};

template<typename Signature>
struct IsVolatileFunction<Signature(&)> : public IsVolatileSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsVolatileFunctionV = IsVolatileFunction<Fn>::value;

// check for volatile member function pointer

template<typename T>
struct IsVolatileMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsVolatileMemberFunctionPointer<Signature(T::*)>
    : public IsVolatileSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsVolatileMemberFunctionPointerV =
    IsVolatileMemberFunctionPointer<MemFn>::value;

// lvalue qualified

namespace detail
{
template<typename Signature>
struct IsLvalueSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsLvalueSignatureHelper<Ret(Args...)&> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsLvalueSignature
    : public std::conditional_t<
          detail::IsLvalueSignatureHelper<
              RemoveSignatureVariadicT<RemoveSignatureConstT<
                  RemoveSignatureVolatileT<RemoveSignatureRvalueT<
                      RemoveSignatureNothrowT<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool IsLvalueSignatureV = IsLvalueSignature<Signature>::value;

template<typename Fn>
struct IsLvalueFunction : public IsLvalueSignature<Fn>
{};

template<typename Signature>
struct IsLvalueFunction<Signature(*)> : public IsLvalueSignature<Signature>
{};

template<typename Signature>
struct IsLvalueFunction<Signature(&)> : public IsLvalueSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsLvalueFunctionV = IsLvalueFunction<Fn>::value;

// check for lvalue member function pointer

template<typename T>
struct IsLvalueMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsLvalueMemberFunctionPointer<Signature(T::*)>
    : public IsLvalueSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsLvalueMemberFunctionPointerV =
    IsLvalueMemberFunctionPointer<MemFn>::value;

// rvalue qualified functions

namespace detail
{
template<typename Signature>
struct IsRvalueSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsRvalueSignatureHelper<Ret(Args...) &&> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsRvalueSignature
    : public std::conditional_t<
          detail::IsRvalueSignatureHelper<
              RemoveSignatureVariadicT<RemoveSignatureConstT<
                  RemoveSignatureVolatileT<RemoveSignatureLvalueT<
                      RemoveSignatureNothrowT<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool IsRvalueSignatureV = IsRvalueSignature<Signature>::value;

template<typename Fn>
struct IsRvalueFunction : public IsRvalueSignature<Fn>
{};

template<typename Signature>
struct IsRvalueFunction<Signature(*)> : public IsRvalueSignature<Signature>
{};

template<typename Signature>
struct IsRvalueFunction<Signature(&)> : public IsRvalueSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsRvalueFunctionV = IsRvalueFunction<Fn>::value;

// check for rvalue member function pointer

template<typename T>
struct IsRvalueMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsRvalueMemberFunctionPointer<Signature(T::*)>
    : public IsRvalueSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsRvalueMemberFunctionPointerV =
    IsRvalueMemberFunctionPointer<MemFn>::value;

// nothrow functions

namespace detail
{
template<typename Signature>
struct IsNothrowSignatureHelper : public std::false_type
{};

template<typename Ret, typename... Args>
struct IsNothrowSignatureHelper<Ret(Args...) const> : public std::true_type
{};
} // namespace detail

template<typename Signature>
struct IsNothrowSignature
    : public std::conditional_t<
          detail::IsNothrowSignatureHelper<
              RemoveSignatureVariadicT<RemoveSignatureConstT<
                  RemoveSignatureVolatileT<RemoveSignatureLvalueT<
                      RemoveSignatureRvalueT<Signature>>>>>>::value,
          std::true_type,
          std::false_type>
{};

template<typename Signature>
inline constexpr bool IsNothrowSignatureV =
    IsNothrowSignature<Signature>::value;

template<typename Fn>
struct IsNothrowFunction : public IsNothrowSignature<Fn>
{};

template<typename Signature>
struct IsNothrowFunction<Signature(*)> : public IsNothrowSignature<Signature>
{};

template<typename Signature>
struct IsNothrowFunction<Signature(&)> : public IsNothrowSignature<Signature>
{};

template<typename Fn>
inline constexpr bool IsNothrowFunctionV = IsNothrowFunction<Fn>::value;

// check for nothrow member function pointer

template<typename T>
struct IsNothrowMemberFunctionPointer : public std::false_type
{};

template<typename T, typename Signature>
struct IsNothrowMemberFunctionPointer<Signature(T::*)>
    : public IsNothrowSignature<Signature>
{};

template<typename MemFn>
inline constexpr bool IsNothrowMemberFunctionPointerV =
    IsNothrowMemberFunctionPointer<MemFn>::value;

// member function pointer class

template<typename Signature>
struct MemberFunctionPointerClass;

template<typename T, typename Signature>
struct MemberFunctionPointerClass<Signature(T::*)>
    : public std::conditional_t<IsSignatureV<Signature>,
                                detail::Identity<T>,
                                detail::Empty>
{};

template<typename Signature>
using MemberFunctionPointerClassT =
    typename MemberFunctionPointerClass<Signature>::type;

// get return type

namespace detail
{
template<typename Sig>
struct SignatureReturnHelper;

template<typename Ret, typename... Args>
struct SignatureReturnHelper<Ret(Args...)>
{
    using type = Ret;
};
} // namespace detail

template<typename Signature>
struct SignatureReturn : public detail::SignatureReturnHelper<
                             sigma::RemoveSignatureQualifiersT<Signature>>
{};

template<typename Signature>
using SignatureReturnT = typename SignatureReturn<Signature>::type;

template<typename Fn>
struct FunctionReturn : public SignatureReturn<Fn>
{};

template<typename Signature>
struct FunctionReturn<Signature(*)> : public SignatureReturn<Signature>
{};

template<typename Signature>
struct FunctionReturn<Signature(&)> : public SignatureReturn<Signature>
{};

template<typename Fn>
using FunctionReturnT = typename FunctionReturn<Fn>::type;

// get member function return type

template<typename T>
struct MemberFunctionPointerReturn;

template<typename T, typename Signature>
struct MemberFunctionPointerReturn<Signature(T::*)>
    : public SignatureReturn<Signature>
{};

template<typename MemFn>
using MemberFunctionPointerReturnT =
    typename MemberFunctionPointerReturn<MemFn>::type;

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