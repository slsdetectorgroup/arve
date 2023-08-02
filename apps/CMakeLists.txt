


add_executable(play main.cpp)
target_link_libraries(play PRIVATE arve::static arve::settings)

set_target_properties(play PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME play
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