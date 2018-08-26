#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sigma/function_ref.hpp"

int test_func(int i)
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

    constexpr long bar(long l)
    {
        return i + l;
    }

    constexpr int const_fun() const
    {
        return 10;
    }
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

        REQUIRE(functor(5) == 10);
        REQUIRE(f.i == 10); // make sure it wasn't moved or anything
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
    }

    SECTION("void return"){
        // static_assert(sigma::is_compatible_function<void(int),
        // int(int)>::value,
        //              "incompatible functions");
        // sigma::FunctionRef<void(int)> void_ret{[](int i) -> int { return i;
        // }};
    };
}
