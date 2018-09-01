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

    constexpr int operator()(int j, int k) const
    {
        return j + k;
    }

    constexpr long bar(long l)
    {
        return i + l;
    }

    constexpr int const_fun() const
    {
        return 10;
    }

    constexpr int const_fun()
    {
        return 5;
    }
};

int baz()
{
    return 0;
}

TEST_CASE("function_ref")
{
    sigma::function_ref<bool(const std::string&)> uninitialized;

    SECTION("default throws")
    {
        CHECK_THROWS_AS(uninitialized("hello world"), std::bad_function_call);
    }

    SECTION("function pointer")
    {
        sigma::function_ref<int(int)> fptr{test_func};

        int  i   = 73;
        auto res = fptr(i);
        REQUIRE(res == 74);
    }

    SECTION("functor")
    {
        foo                           f{};
        sigma::function_ref<int(int)> functor{f};

        // using the non const operator()
        CHECK(functor(5) == 10);
        CHECK(f.i == 10); // make sure it wasn't moved or anything

        const foo                     g{};
        sigma::function_ref<int(int)> const_functor{g};

        // using the operator() const
        CHECK(const_functor(5) == 5);
        CHECK(g.i == 5);
    }

    SECTION("member function")
    {
        foo  g{};
        auto mem_fn_from_ref =
            sigma::function_ref<long(long)>::bind<&foo::bar>(g);

        CHECK(mem_fn_from_ref(0) == 5);

        // call from const member function
        const foo b{};
        auto      const_mem_fn =
            sigma::function_ref<int()>::bind<sigma::overload<int() const>(
                &foo::const_fun)>(b);

        CHECK(const_mem_fn() == 10); // call the const version which returns 10

        // will call the non const version and return 5;
        auto nonconst_fun =
            sigma::function_ref<int()>::bind<sigma::overload<int()>(
                &foo::const_fun)>(g);

        CHECK(nonconst_fun() ==
              5); // call the non const version which returns 5

        CHECK(sigma::is_member_function_pointer_const_v<decltype(
                  sigma::overload<int() const>(&foo::const_fun))>);
        CHECK_FALSE(
            sigma::is_member_function_pointer_const_v<decltype(&foo::bar)>);
    }

    SECTION("operator bool")
    {
        sigma::function_ref<int(int)> valid{test_func};
        CHECK(valid);
        CHECK_FALSE(uninitialized);
    }

    SECTION("noexcept")
    {
        sigma::function_ref<int(int)>          throwing_fn{test_func};
        sigma::function_ref<int(int) noexcept> nothrow_fn{test_func_nothrow};

        CHECK(sigma::is_signature_nothrow_v<decltype(
                  nothrow_fn)::signature_type>);
        CHECK(decltype(nothrow_fn)::is_nothrow);

        CHECK_FALSE(sigma::is_nothrow_invocable_v<decltype(throwing_fn), int>);
        CHECK(sigma::is_nothrow_invocable_v<decltype(nothrow_fn), int>);
    }

    SECTION("bind own object")
    {
        // since there is one pointer for the function pointer and one for an
        // instance pointer, we can bind any object we want there. This tests
        // that functionality

        int i{0}; // i to be changed

        struct simple_obj
        {
            std::reference_wrapper<int> i;

            simple_obj(int& i) : i{i}
            {}

            void func(int i, double d, float f)
            {
                this->i.get() += i;
            }
        };

        simple_obj o{i};

        sigma::function_ref<void(int, double, float)> bind_object =
            sigma::function_ref<void(
                int, double, float)>::bind<&simple_obj::func>(o);

        bind_object(5, 1.0, 1.0f);

        CHECK(i == 5);
    }

    SECTION("void return"){
        // static_assert(sigma::is_compatible_function<void(int),
        // int(int)>::value,
        //              "incompatible functions");
        // sigma::function_ref<void(int)> void_ret{[](int i) -> int { return i;
        // }};
    };
}
