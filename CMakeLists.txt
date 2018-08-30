cmake_minimum_required(VERSION 3.2)
project(Sigma VERSION 0.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

option(SIGMA_INCLUDE_TESTS "Generate build targets for the unit tests." ON)

find_package(Catch2 )

add_subdirectory(sigma)

if(SIGMA_INCLUDE_TESTS)
enable_testing()
add_subdirectory(tests)
endif(SIGMA_INCLUDE_TESTS)