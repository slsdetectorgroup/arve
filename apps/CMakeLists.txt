


add_executable(play main.cpp)
target_link_libraries(play PRIVATE arve::static arve::settings)

set_target_properties(play PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_NAME play
)