# Only sources that don't depend on the 
# detector iterface for portability
set(PythonCppSources
    src/main.cpp 
    src/frame_grabber.cpp
)

pybind11_add_module(_arve
    ${PythonCppSources}
)

target_link_libraries(_arve PRIVATE arve::settings arve::static)



set_target_properties(_arve PROPERTIES
                        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
                    )

#Copy python souce files
set(PythonFiles
    __init__.py
)

foreach(FILE ${PythonFiles})
    configure_file( arve/${FILE}  
    ${CMAKE_BINARY_DIR}/arve/${FILE})
endforeach(FILE ${PYTHON_FILES})

#Copy python "executables"
set(PythonApps
    play.py
)

foreach(FILE ${PythonApps})
    configure_file( apps/${FILE}  
    ${CMAKE_BINARY_DIR}/${FILE})
endforeach(FILE ${PYTHON_FILES})

#Python test files 
# set(PythonTests
#     test_load_channel_mask.py
#     channel_mask.txt
# )

# foreach(FILE ${PythonTests})
#     configure_file( tests/${FILE}  
#     ${CMAKE_BINARY_DIR}/python_tests/${FILE})
# endforeach(FILE ${PYTHON_FILES})