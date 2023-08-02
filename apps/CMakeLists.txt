


add_executable(receiver receiver.cpp)
target_link_libraries(receiver PRIVATE arve::static arve::settings)

set_target_properties(receiver PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME receiver
)

add_executable(scratch scratch.cpp)
target_link_libraries(scratch PRIVATE arve::static arve::settings)

set_target_properties(scratch PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME scratch
)

add_executable(counter counter.cpp)
target_link_libraries(counter PRIVATE arve::static arve::settings)

set_target_properties(counter PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME counter
)