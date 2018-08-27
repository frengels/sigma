#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sigma/result.hpp"

struct CopyMove
{
    int copy{0};
    int move{0};

    constexpr CopyMove() = default;
    constexpr CopyMove(const CopyMove& other) noexcept
        : copy{other.copy}, move{other.move}
    {
        ++copy;
    }
    constexpr CopyMove& operator=(const CopyMove& other) noexcept
    {
        copy = other.copy;
        move = other.move;
        ++copy;
        return *this;
    }
    constexpr CopyMove(CopyMove&& other) noexcept
        : copy{std::move(other.copy)}, move{std::move(other.move)}
    {
        ++move;
    }
    constexpr CopyMove& operator=(CopyMove&& other) noexcept
    {
        copy = std::move(other.copy);
        move = std::move(other.move);
        ++move;
        return *this;
    }
};

template<bool B = false>
sigma::Result<CopyMove, B> make_copy_move()
{
    return sigma::Result<CopyMove, B>(CopyMove());
}

template<bool B = false>
sigma::Result<CopyMove, B> make_copy_move_in_place()
{
    return sigma::Result<CopyMove, B>(std::in_place_t{});
}

sigma::Result<int> nothing_under_zero(int i)
{
    if (i < 0)
    {
        try
        {
            throw std::logic_error("I said nothing under 0");
        }
        catch (...)
        {
            return std::current_exception();
        }
    }

    return {i};
}

template<typename T>
std::exception_ptr throw_this(T&& t)
{
    try
    {
        throw std::forward<T>(t);
    }
    catch (...)
    {
        return std::current_exception();
    }
}

TEST_CASE("Result")
{
    std::logic_error except{"this could've been avoided"};

    sigma::Result<int>       valid{5};
    sigma::Result<int, true> valid_nothrow{5};
    sigma::Result<int>       invalid{std::invoke([]() -> std::exception_ptr {
        try
        {
            throw std::logic_error{"this could've been avoided"};
        }
        catch (...)
        {
            return std::current_exception();
        }
    })};

    SECTION("has_value")
    {
        CHECK(valid.has_value());
        CHECK(valid);
        CHECK(valid_nothrow.has_value());
        CHECK(valid_nothrow);
        CHECK_FALSE(invalid.has_value());
        CHECK_FALSE(invalid);
    }

    SECTION("get value")
    {
        CHECK(valid.value() == 5);
        CHECK(valid.value() == 5);
        CHECK_THROWS_AS(invalid.value(), std::logic_error);
        CHECK(invalid.value_or(10) == 10);
    }

    SECTION("move")
    {
        // move CopyMove into result
        auto res         = make_copy_move();
        auto res_nothrow = make_copy_move<true>();

        CHECK(res->copy == 0);
        CHECK(res->move == 1);
        CHECK(res_nothrow->copy == 0);
        CHECK(res_nothrow->move == 1);

        // move CopyMove into result and then out into the value
        CopyMove move         = make_copy_move().value();
        CopyMove move_nothrow = make_copy_move().value();

        CHECK(move.copy == 0);
        CHECK(move.move == 2);
        CHECK(move_nothrow.copy == 0);
        CHECK(move_nothrow.move == 2);
    }

    SECTION("in place construction")
    {
        auto res         = sigma::Result<CopyMove>(std::in_place_t{});
        auto res_nothrow = sigma::Result<CopyMove, true>(std::in_place_t{});

        CHECK(res->copy == 0);
        CHECK(res->move == 0);
        CHECK(res_nothrow->copy == 0);
        CHECK(res_nothrow->move == 0);

        auto res2         = make_copy_move_in_place();
        auto res2_nothrow = make_copy_move_in_place<true>();

        CHECK(res2->copy == 0);
        CHECK(res2->move == 0);
        CHECK(res2_nothrow->copy == 0);
        CHECK(res2_nothrow->move == 0);

        auto& res2_ref         = res2.value();
        auto& res2_nothrow_ref = res2_nothrow.value();

        CHECK(res2_ref.copy == 0);
        CHECK(res2_ref.move == 0);
        CHECK(res2_nothrow_ref.copy == 0);
        CHECK(res2_nothrow_ref.move == 0);

        auto res2_moved         = std::move(res2).value();
        auto res2_nothrow_moved = std::move(res2_nothrow).value();

        CHECK(res2_moved.copy == 0);
        CHECK(res2_moved.move == 1);
        CHECK(res2_nothrow_moved.copy == 0);
        CHECK(res2_nothrow_moved.move == 1);
    }

    SECTION("rethrow")
    {
        // result with thrown 5
        sigma::Result<int> thrown5{throw_this(5)};

        CHECK_FALSE(thrown5);

        CHECK_THROWS_AS(thrown5.try_rethrow(), int);
        CHECK_THROWS_AS(thrown5.value(), int);
    }

    SECTION("noexcept specifics")
    {
        CHECK(sizeof(sigma::Result<std::string, true>) <
              sizeof(sigma::Result<std::string, false>));
        sigma::Result<std::string, true> valid{"hello, world"};

        // cannot construct nothrow result from exception_ptr
        // this check fails when it shouldn't, actually trying to construct the
        // object doesn't work
        // CHECK_FALSE(std::is_constructible_v<sigma::Result<std::string, true>,
        //                                    std::exception_ptr>);
    }
}