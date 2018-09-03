add_library(sigma INTERFACE)
target_include_directories(sigma INTERFACE ${CMAKE_SOURCE_DIR})
target_link_libraries(sigma INTERFACE Boost::boost)