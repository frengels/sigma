#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sigma/signal.hpp"

struct Functor
{
    std::reference_wrapper<int> copy;
    std::reference_wrapper<int> move;

    Functor(int& c, int& m) noexcept : copy{c}, move{m}
    {}
    Functor(const Functor& other) noexcept : copy{other.copy}, move{other.move}
    {
        ++copy;
    }
    Functor(Functor&& other) noexcept
        : copy{std::move(other.copy)}, move{std::move(other.move)}
    {
        ++move;
    }

    Functor& operator=(const Functor& other) noexcept
    {
        copy = other.copy;
        move = other.move;

        ++copy;
        return *this;
    }

    Functor& operator=(Functor&& other) noexcept
    {
        copy = std::move(other.copy);
        move = std::move(other.move);

        ++move;
        return *this;
    }

    void operator()(int)
    {}
};

TEST_CASE("Connections")
{
    sigma::signal<void(int)> test;

    SECTION("empty signal")
    {
        test(5);
    }

    SECTION("function pointer signal")
    {
        // simply testing whether construction works
        test.connect([](auto i) {});
    }

    SECTION("connect stateful lambda")
    {
        // cannot connect stateful lambdas using function_ref
        // TODO make interface which allows std::function to be used
        /*
        int i{5};
        test.connect([&](int) { ++i; });
        test(5);

        CHECK(i == 6);
        */
    }

    SECTION("functor move")
    {
        // functor cannot be moved into sigma::function_ref as it's simply a
        // reference
        /*
        int copy = 5;
        int move = 5;

        test.connect(Functor{copy, move});

        // not sure why 2 moves are involved but whatever
        CHECK(copy == 5);
        CHECK(move == 7);
        */
    }

    SECTION("functor copy")
    {

        int copy = 5;
        int move = 5;

        Functor f{copy, move};
        test.connect(f);

        // don't know why both the move happens in this case

        // resources only get moved and copied with std::function
        // CHECK(copy == 6);
        // CHECK(move == 6);

        // with sigma::function_ref only a reference is taken
        CHECK(copy == 5);
        CHECK(move == 5);
    }
}