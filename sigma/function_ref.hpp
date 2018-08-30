#ifndef SIGMA_FUNCTION_REF_HPP_INCLUDED
#define SIGMA_FUNCTION_REF_HPP_INCLUDED

#pragma once

#include <cstdio>
#include <exception>
#include <functional>
#include <type_traits>

#include "sigma/function_traits.hpp"

namespace sigma
{

template<bool Nothrow, typename Signature>
struct FunctionRefTraits;

/**
 * helper for basic functionality needed by FunctionRef,
 * signature is without void*
 */
template<bool Nothrow, typename Ret, typename... Args>
struct FunctionRefTraits<Nothrow, Ret(Args...)>
{
    static constexpr bool is_nothrow = Nothrow;
    using return_type                = Ret;

    /**
     * default function used with the default constructor
     */
    [[noreturn]] static constexpr return_type
    default_uninit(void*, Args...) noexcept(is_nothrow)
    {
        if constexpr (is_nothrow)
        {
            fprintf(stderr,
                    "bad_function_call on FunctionRefTraits, tried to call "
                    "empty FunctionRef\n");
            std::terminate();
        }
        else
        {
            throw std::bad_function_call{};
        }
    };

    static constexpr return_type call_fn_ptr(void* f,
                                             Args... args) noexcept(is_nothrow)
    {
        using fn_ptr_type = return_type (*)(Args...) noexcept(is_nothrow);
        auto fn_ptr       = reinterpret_cast<fn_ptr_type>(f);
        return std::invoke(fn_ptr, std::forward<Args>(args)...);
    }

    template<typename Callable>
    static constexpr return_type call_functor(void* obj,
                                              Args... args) noexcept(is_nothrow)
    {
        Callable& callable = *static_cast<Callable*>(obj);

        return std::invoke(callable, std::forward<Args>(args)...);
    }

    template<auto MemFn, typename T>
    static constexpr return_type call_mem_fn(void* obj,
                                             Args... args) noexcept(is_nothrow)
    {
        T&   instance = *static_cast<T*>(obj);
        auto mem_fn   = MemFn;
        return std::invoke(mem_fn, instance, std::forward<Args>(args)...);
    }
};

template<typename Signature>
class FunctionRef final {
    /*
     * the function signature can have a variadic and noexcept qualifier,
     * nothing else
     */
    static_assert(sigma::IsSignatureV<Signature>,
                  "Signature is not a valid function signature");
    static_assert(!sigma::IsConstSignatureV<Signature>,
                  "Function signature cannot be const qualified");
    static_assert(!sigma::IsVolatileSignatureV<Signature>,
                  "Function signature cannot be volatile qualified");
    static_assert(!sigma::IsLvalueSignatureV<Signature>,
                  "Function signature cannot be lvalue qualified");
    static_assert(!sigma::IsRvalueSignatureV<Signature>,
                  "Function signature cannot be rvalue qualified");

public:
    static constexpr bool is_nothrow = sigma::IsNothrowSignatureV<Signature>;
    using return_type                = sigma::SignatureReturnT<Signature>;
    /**
     * signature of the function stored in this FunctionRef type, can be seen as
     * return_type (Args...) noexcept(is_nothrow)
     */
    using signature_type = Signature;

    using traits_type =
        FunctionRefTraits<is_nothrow,
                          sigma::RemoveSignatureQualifiersT<signature_type>>;

    /**
     * function signature without noexcept specifier, used as intermediate
     * state. in the form return_type(void*, Args...) noexcept(false)
     */
    using _unqualified_callback_signature = sigma::MakeSignatureT<
        return_type(void*, sigma::SignatureParametersAsTupleT<signature_type>)>;
    /**
     * function pointer in the form of return_type (*)(void*, Args...)
     * noexcept(true/false)
     */
    using callback_type = std::add_pointer_t<std::conditional_t<
        is_nothrow,
        sigma::AddSignatureNothrowT<_unqualified_callback_signature>,
        sigma::RemoveSignatureNothrowT<_unqualified_callback_signature>>>;

private:
    void*               m_instance{nullptr};
    const callback_type m_callback{&traits_type::default_uninit};

private:
    /**
     * constructor for the member functions
     */
    template<typename T>
    constexpr FunctionRef(T* obj, callback_type cb) noexcept
        : m_instance{reinterpret_cast<void*>(
              const_cast<std::remove_const_t<T>*>(obj))},
          m_callback{cb}
    {}

public:
    /**
     * default initializers which initialize with default uninit
     */
    constexpr FunctionRef() noexcept = default;
    constexpr FunctionRef(std::nullptr_t) noexcept : FunctionRef{}
    {}
    /**
     * initialize with a simple function pointer
     */
    constexpr FunctionRef(std::add_pointer_t<signature_type> fn_ptr) noexcept
        : m_instance{reinterpret_cast<void*>(fn_ptr)},
          m_callback{&traits_type::call_fn_ptr}
    {}

    /**
     * copy constructors
     */
    constexpr FunctionRef(const FunctionRef&) noexcept = default;
    constexpr FunctionRef& operator=(const FunctionRef&) noexcept = default;

    template<typename Callable,
             std::enable_if_t<!std::is_function_v<Callable>, int> = 0>
    constexpr explicit FunctionRef(Callable& callable) noexcept
        : FunctionRef{std::addressof(callable),
                      &traits_type::template call_functor<Callable>}
    {}

    template<typename... Args>
    constexpr return_type operator()(Args&&... args) const noexcept(is_nothrow)
    {
        return std::invoke(m_callback, m_instance, std::forward<Args>(args)...);
    }

    constexpr operator bool() const noexcept
    {
        return m_callback != &traits_type::default_uninit;
    }

    /**
     * create a function reference to a member function
     * the function reference can be created as follows
     * sigma::FunctionRef<Signature>::member_fn<&class::mem_fn>(instance);
     */
    template<auto MemFn, typename T>
    static constexpr std::enable_if_t<
        std::is_class_v<T> && sigma::IsMemberFunctionPointerV<decltype(MemFn)>,
        FunctionRef>
    member_fn(T& obj) noexcept
    {
        static_assert(sigma::IsMemberFunctionPointerV<decltype(MemFn)>,
                      "MemFn must be a member function pointer");
        using mem_fn_class_type =
            sigma::MemberFunctionPointerClassT<decltype(MemFn)>;
        static_assert(std::is_base_of_v<mem_fn_class_type, T>,
                      "Member function pointer class "
                      "is not a base of T");
        static_assert(
            !is_nothrow ||
                sigma::IsNothrowMemberFunctionPointerV<decltype(MemFn)>,
            "If the FunctionRef is specified as nothrow then the "
            "member function pointer must have a noexcept "
            "qualifier");

        return FunctionRef{std::addressof(obj),
                           &traits_type::template call_mem_fn<MemFn, T>};
    }
};
} // namespace sigma

#endif