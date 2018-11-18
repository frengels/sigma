function(sigma_add_test )
endfunction()

add_library(catchlib STATIC catch_lib.cpp)

add_executable(test_connection test_connection.cpp)
target_include_directories(test_connection PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_connection catchlib sigma)
add_test(connection test_connection)

add_executable(test_function_ref test_function_ref.cpp)
target_include_directories(test_function_ref PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_function_ref catchlib)
add_test(function_ref test_function_ref)

add_executable(test_result test_result.cpp)
target_include_directories(test_result PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_result catchlib)
add_test(result test_result)

add_executable(test_function_traits test_function_traits.cpp)
target_include_directories(test_function_traits PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_function_traits catchlib)
add_test(function_traits test_function_traits)

add_executable(test_handle_vector test_handle_vector.cpp)
target_include_directories(test_handle_vector PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_handle_vector catchlib)
add_test(handle_vector test_handle_vector)
