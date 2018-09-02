add_library(sigma STATIC connection.cpp)
target_include_directories(sigma PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(sigma Boost::boost)