cmake_minimum_required(VERSION 3.17)
project(arve)
include(GNUInstallDirs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(CMAKE_INSTALL_RPATH $ORIGIN)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

#Force Release if build is not specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()


option(ARVE_TUNE_LOCAL "Tune to the specific CPU" OFF)
option(ARVE_USE_SANITIZER "Use address sanitized" OFF)
option(ARVE_BUILD_TESTS "Build unit tests" OFF)
option(ARVE_BUILD_PYTHON_EXT "Build python extension OFF")

add_library(project_settings INTERFACE)
target_compile_features(project_settings INTERFACE cxx_std_17)
target_compile_options(project_settings INTERFACE 
                            -Wall
                            -Wextra
                            # -Wshadow
                            -Wunused
                            -Wno-unused-parameter
                            -Wold-style-cast
                            -Wcast-align
                            -Wnon-virtual-dtor
                            -Woverloaded-virtual
                            -Wdouble-promotion
                            -Wformat=2
                            -Wredundant-decls
                            -Wconversion
                            -Wnull-dereference
                            -Wdouble-promotion
                            -Werror=return-type
                            -Wpedantic
                            -Wcast-align
)
if(ARVE_USE_SANITIZER)
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
  set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
  set(CMAKE_BUILD_TYPE "Debug")
endif(ARVE_USE_SANITIZER)

if(ARVE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()


add_library(arve::settings ALIAS project_settings)

if(ARVE_TUNE_LOCAL)
    target_compile_options(project_settings INTERFACE -mtune=native -march=native)
endif()

#Deps
find_package(fmt REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



#Add library
set(PublicHeaders
    include/arve/ProducerConsumerQueue.h
    include/arve/UdpSocket.h
)

set(SourceFiles
    src/PacketSource.cpp
    src/terminal.cpp
    src/UdpSocket.cpp
    src/utils.cpp
)

if(ARVE_BUILD_TESTS)
    target_sources(
        tests PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameGrabber.test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CircularFifo.test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ProducerConsumerQueue.test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RawFrame.test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/UdpSocket.test.cpp
    )
endif()

add_library(
    arve_object OBJECT ${SourceFiles} 
)

target_link_libraries(arve_object PRIVATE arve::settings fmt::fmt Threads::Threads )
target_include_directories(arve_object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(arve_object PROPERTIES
    LIBRARY_OUTPUT_NAME arve_object
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    POSITION_INDEPENDENT_CODE ON
    PUBLIC_HEADER "${PublicHeaders}"
)


#Shared library
add_library(arve_shared SHARED $<TARGET_OBJECTS:arve_object>)
target_link_libraries(arve_shared PUBLIC arve_object)
set_target_properties(arve_shared PROPERTIES
    LIBRARY_OUTPUT_NAME arve
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    # PUBLIC_HEADER "${PUBLICHEADERS}"
)

#Static library
add_library(arve_static STATIC $<TARGET_OBJECTS:arve_object>)
target_link_libraries(arve_static PUBLIC arve_object)
set_target_properties(arve_static PROPERTIES
    ARCHIVE_OUTPUT_NAME arve
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    # PUBLIC_HEADER "${PUBLICHEADERS}"
)

add_library(arve::static ALIAS arve_static)
add_library(arve::shared ALIAS arve_shared)


add_subdirectory(apps)

if(ARVE_BUILD_PYTHON_EXT)
  #Take python from conda, TODO! deal with non virtualenv
  set(Python_FIND_VIRTUALENV "ONLY" CACHE STRING "Only virtual envs")
  find_package (Python 3.6 COMPONENTS Interpreter Development)
  find_package(pybind11 CONFIG)
  add_subdirectory(python)
endif()
