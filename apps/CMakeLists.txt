


add_executable(tester main.cpp)
target_link_libraries(tester PRIVATE arve::static arve::settings)

set_target_properties(tester PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME tester
)