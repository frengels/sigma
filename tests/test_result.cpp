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

sigma::Result<CopyMove> make_copy_move()
{
    return sigma::Result<CopyMove>(CopyMove());
}

sigma::Result<CopyMove> make_copy_move_in_place()
{
    return sigma::Result<CopyMove>(std::in_place_t{});
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

TEST_CASE("Result")
{
    std::logic_error except{"this could've been avoided"};

    sigma::Result<int> valid{5};
    sigma::Result<int> invalid{std::invoke([]() -> std::exception_ptr {
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
        REQUIRE(valid.has_value());
        REQUIRE(valid);
        REQUIRE_FALSE(invalid.has_value());
        REQUIRE_FALSE(invalid);
    }

    SECTION("get value")
    {
        REQUIRE(valid.value() == 5);
        REQUIRE_THROWS_AS(invalid.value(), std::logic_error);
        REQUIRE(invalid.value_or(10) == 10);
    }

    SECTION("move")
    {
        // move CopyMove into result
        auto res = make_copy_move();

        REQUIRE(res->copy == 0);
        REQUIRE(res->move == 1);

        // move CopyMove into result and then out into the value
        CopyMove move = make_copy_move().value();

        REQUIRE(move.copy == 0);
        REQUIRE(move.move == 2);
    }

    SECTION("in place construction")
    {
        auto res = sigma::Result<CopyMove>(std::in_place_t{});

        REQUIRE(res->copy == 0);
        REQUIRE(res->move == 0);

        auto res2 = make_copy_move_in_place();

        REQUIRE(res2->copy == 0);
        REQUIRE(res2->move == 0);

        auto& res2_ref = res2.value();

        REQUIRE(res2_ref.copy == 0);
        REQUIRE(res2_ref.move == 0);

        auto res2_moved = std::move(res2).value();

        REQUIRE(res2_moved.copy == 0);
        REQUIRE(res2_moved.move == 1);
    }

    SECTION("rethrow")
    {
        auto generate_exception_ptr = [](auto obj) -> std::exception_ptr {
            try
            {
                throw obj;
            }
            catch (...)
            {
                return std::current_exception();
            }
        };

        // result with thrown 5
        sigma::Result<int> thrown5{generate_exception_ptr(5)};

        REQUIRE_FALSE(thrown5);

        REQUIRE_THROWS_AS(thrown5.try_rethrow(), int);
        REQUIRE_THROWS_AS(thrown5.value(), int);
    }
}