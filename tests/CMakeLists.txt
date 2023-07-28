
find_package(Catch2 3 REQUIRED)

add_executable(tests main.cpp)


target_link_libraries(tests PRIVATE Catch2::Catch2WithMain arve::shared)
set_target_properties(tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME run_tests
)

include(CTest)
include(Catch)
catch_discover_tests(tests)