#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sigma/function_traits.hpp"

void normal()
{}
void variadic(...)
{}
void nothrow() noexcept
{}

void nth(int)
{}

struct foo
{
    void normal()
    {}
    void normal_c() const
    {}
    void normal_v() volatile
    {}
    void normal_cv() const volatile
    {}

    void var(...)
    {}
    void var_c(...) const
    {}
    void var_v(...) volatile
    {}
    void var_cv(...) const volatile
    {}

    void lvalue() &
    {}
    void lv_c() const&
    {}
    void lv_v() volatile&
    {}
    void lv_cv() const volatile&
    {}

    void lv_var(...) &
    {}
    void lv_var_c(...) const&
    {}
    void lv_var_v(...) volatile&
    {}
    void lv_var_cv(...) const volatile&
    {}

    void rvalue() &&
    {}
    void rv_c() const&&
    {}
    void rv_v() volatile&&
    {}
    void rv_cv() const volatile&&
    {}

    void rv_var(...) &&
    {}
    void rv_var_c(...) const&&
    {}
    void rv_var_v(...) volatile&&
    {}
    void rv_var_cv(...) const volatile&&
    {}

    // noexcept

    void nnormal() noexcept
    {}
    void nnormal_c() const noexcept
    {}
    void nnormal_v() volatile noexcept
    {}
    void nnormal_cv() const volatile noexcept
    {}

    void nvar(...) noexcept
    {}
    void nvar_c(...) const noexcept
    {}
    void nvar_v(...) volatile noexcept
    {}
    void nvar_cv(...) const volatile noexcept
    {}

    void nlv() & noexcept
    {}
    void nlv_c() const& noexcept
    {}
    void nlv_v() volatile& noexcept
    {}
    void nlv_cv() const volatile& noexcept
    {}

    void nlv_var(...) & noexcept
    {}
    void nlv_var_c(...) const& noexcept
    {}
    void nlv_var_v(...) volatile& noexcept
    {}
    void nlv_var_cv(...) const volatile& noexcept
    {}

    void nrv() && noexcept
    {}
    void nrv_c() const&& noexcept
    {}
    void nrv_v() volatile&& noexcept
    {}
    void nrv_cv() const volatile&& noexcept
    {}

    void nrv_var(...) && noexcept
    {}
    void nrv_var_c(...) const&& noexcept
    {}
    void nrv_var_v(...) volatile&& noexcept
    {}
    void nrv_var_cv(...) const volatile&& noexcept
    {}
};

int bar()
{
    return 0;
}

/* guess it's not part of the function pointer type system
int bar() noexcept
{
    return 1;
}
*/

struct Overloaded
{
    int foo()
    {
        return 0;
    }
    int foo() const
    {
        return 1;
    }
    int foo() volatile
    {
        return 2;
    }
    int foo() const volatile
    {
        return 3;
    }
};

TEST_CASE("FunctionTraits")
{
    SECTION("IsFunction")
    {
        // regular type is not a function
        CHECK_FALSE(sigma::IsFunction<std::string>::value);

        // all decltype variants work
        CHECK(sigma::IsFunction<decltype(variadic)>::value);
        CHECK(sigma::IsFunction<decltype(*variadic)>::value);
        CHECK(sigma::IsFunction<decltype(&variadic)>::value);

        // non member functions
        CHECK(sigma::IsFunction<decltype(normal)>::value);
        CHECK(sigma::IsFunction<decltype(variadic)>::value);
        CHECK(sigma::IsFunction<decltype(nothrow)>::value);

        // member functions
        CHECK(sigma::IsFunctionV<decltype(&foo::nrv_cv)>);
        CHECK(sigma::IsMemberFunctionPointerV<decltype(&foo::rv_cv)>);
    }

    SECTION("member functions")
    {
        CHECK_FALSE(sigma::IsConstFunctionV<decltype(variadic)>);

        CHECK(sigma::IsConstFunctionV<decltype(&foo::nrv_var_cv)>);
    }

    SECTION("nth parameter")
    {
        CHECK(std::is_same_v<sigma::NthElementT<2, void, int, float>, float>);
        CHECK(std::is_same_v<sigma::NthParameterT<0, decltype(&nth)>, int>);
    }

    SECTION("signature")
    {
        CHECK(std::is_same_v<void() & noexcept,
                             sigma::FunctionSignatureT<decltype(&foo::nlv)>>);
        CHECK_FALSE(
            std::is_same_v<void() noexcept,
                           sigma::FunctionSignatureT<decltype(&foo::nlv)>>);
    }

    SECTION("selecting overloads")
    {
        // CHECK(sigma::Overload<int() noexcept>{bar}() == 1);
    }

    SECTION("class from mem_fn")
    {
        CHECK(std::is_same_v<
              foo,
              sigma::MemberFunctionPointerClassT<decltype(&foo::nrv_cv)>>);
    }
}