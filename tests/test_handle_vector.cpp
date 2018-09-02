#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sigma/handle_vector.hpp"

TEST_CASE("handle_vector")
{
    sigma::handle_vector<int> integers;

    auto handle1 = integers.emplace_back(1);
    auto handle2 = integers.emplace_back(2);
    auto handle3 = integers.emplace_back(3);

    SECTION("retrieving")
    {
        CHECK(integers[handle1] == 1);
        CHECK(integers[handle2] == 2);
        CHECK(integers[handle3] == 3);
    }

    SECTION("removing")
    {
        integers.erase(handle1);

        CHECK_THROWS_AS(integers[handle1], sigma::bad_handle_access);
        CHECK(integers[handle2] == 2);
        CHECK(integers[handle3] == 3);
    }
}