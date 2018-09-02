add_library(sigma STATIC connection.cpp)
target_include_directories(sigma PRIVATE ${CMAKE_SOURCE_DIR})