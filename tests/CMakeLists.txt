function(sigma_add_test )
endfunction()

add_executable(test_connection test_connection.cpp)
target_include_directories(test_connection PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_connection Catch2::Catch2)
add_test(connection test_connection)

add_executable(test_function_ref test_function_ref.cpp)
target_include_directories(test_function_ref PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_function_ref Catch2::Catch2)
add_test(function_ref test_function_ref)

add_executable(test_result test_result.cpp)
target_include_directories(test_result PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_result Catch2::Catch2)
add_test(result test_result)

add_executable(test_function_traits test_function_traits.cpp)
target_include_directories(test_function_traits PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_function_traits Catch2::Catch2)
add_test(function_traits test_function_traits)