#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sigma/function_ref.hpp"

int test_func(int i)
{
    return ++i;
}

int test_func_nothrow(int i) noexcept
{
    return ++i;
}

struct foo
{
    int i{5};

    constexpr foo() = default;

    constexpr int operator()(int j)
    {
        i += j;
        return i;
    }

    constexpr int operator()(int j) const
    {
        return j;
    }

    constexpr long bar(long l)
    {
        return i + l;
    }

    constexpr int const_fun() const
    {
        return 10;
    }

    /*
        constexpr int const_fun()
        {
            return 5;
        }
        */
};

TEST_CASE("FunctionRef")
{
    sigma::FunctionRef<bool(const std::string&)> uninitialized;

    SECTION("default throws")
    {
        CHECK_THROWS_AS(uninitialized("hello world"), std::bad_function_call);
    }

    SECTION("function pointer")
    {
        sigma::FunctionRef<int(int)> fptr{test_func};

        int  i   = 73;
        auto res = fptr(i);
        REQUIRE(res == 74);
    }

    SECTION("functor")
    {
        foo                          f{};
        sigma::FunctionRef<int(int)> functor{f};

        // using the non const operator()
        REQUIRE(functor(5) == 10);
        REQUIRE(f.i == 10); // make sure it wasn't moved or anything

        const foo                    g{};
        sigma::FunctionRef<int(int)> const_functor{g};

        // using the operator() const
        REQUIRE(const_functor(5) == 5);
        REQUIRE(g.i == 5);
    }

    SECTION("member function")
    {
        foo  g{};
        auto mem_fn = sigma::FunctionRef<long(long)>::member_fn<&foo::bar>(&g);
        auto mem_fn_from_ref =
            sigma::FunctionRef<long(long)>::member_fn<&foo::bar>(g);

        REQUIRE(mem_fn(5) == 10);
        REQUIRE(mem_fn_from_ref(0) == 5);

        // call from const member function
        const foo b{};
        auto      const_mem_fn =
            sigma::FunctionRef<int()>::member_fn<&foo::const_fun>(b);

        REQUIRE(const_mem_fn() == 10);

        auto const_mem_fn_ptr =
            sigma::FunctionRef<int()>::member_fn<&foo::const_fun>(&b);
        REQUIRE(const_mem_fn_ptr() == 10);

        // will call the non const version and return 5;
        auto nonconst_fun =
            sigma::FunctionRef<int()>::member_fn<&foo::const_fun>(g);

        REQUIRE(nonconst_fun() == 5);

        static_assert(sigma::is_const_member_function_pointer<decltype(
                          &foo::const_fun)>::value);
        static_assert(!sigma::is_const_member_function_pointer<decltype(
                          &foo::bar)>::value);
    }

    SECTION("operator bool")
    {
        sigma::FunctionRef<int(int)> valid{test_func};
        REQUIRE(valid);
        REQUIRE_FALSE(uninitialized);
    }

    SECTION("noexcept")
    {
        sigma::FunctionRef<int(int)> throwing_fn{test_func};
        sigma::FunctionRef<int(int)> nothrow_fn{test_func_nothrow};

        static_assert(!std::is_nothrow_invocable_v<decltype(throwing_fn), int>);
        // maybe some day
        // static_assert(std::is_nothrow_invocable_v<decltype(nothrow_fn),
        // int>);
    }

    SECTION("void return"){
        // static_assert(sigma::is_compatible_function<void(int),
        // int(int)>::value,
        //              "incompatible functions");
        // sigma::FunctionRef<void(int)> void_ret{[](int i) -> int { return i;
        // }};
    };
}
