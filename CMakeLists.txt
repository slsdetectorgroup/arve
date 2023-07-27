cmake_minimum_required(VERSION 3.17)
project(arve)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(CMAKE_INSTALL_RPATH $ORIGIN)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)


option(RECV_TUNE_LOCAL "Tune to the specific CPU" OFF)

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
add_library(arve::settings ALIAS project_settings)

if(RECV_TUNE_LOCAL)
    target_compile_options(project_settings INTERFACE -mtune=native -march=native)
endif()

#Deps
find_package(fmt REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



#Add library
set(PublicHeaders
    include/arve/SimpleQueue.h
    include/arve/UdpSocket.h
)

set(SourceFiles
  src/UdpSocket.cpp
)

add_library(
    recv_object OBJECT ${SourceFiles} 
)

target_link_libraries(recv_object PRIVATE arve::settings fmt::fmt Threads::Threads )
target_include_directories(recv_object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(recv_object PROPERTIES
    LIBRARY_OUTPUT_NAME recv_object
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    POSITION_INDEPENDENT_CODE ON
    PUBLIC_HEADER "${PublicHeaders}"
)


#Shared library
add_library(recv_shared SHARED $<TARGET_OBJECTS:recv_object>)
target_link_libraries(recv_shared PUBLIC recv_object)
set_target_properties(recv_shared PROPERTIES
    LIBRARY_OUTPUT_NAME arve
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    # PUBLIC_HEADER "${PUBLICHEADERS}"
)

#Static library
add_library(recv_static STATIC $<TARGET_OBJECTS:recv_object>)
target_link_libraries(recv_static PUBLIC recv_object)
set_target_properties(recv_static PROPERTIES
    ARCHIVE_OUTPUT_NAME arve
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    # PUBLIC_HEADER "${PUBLICHEADERS}"
)

add_library(arve::static ALIAS recv_static)
add_library(arve::shared ALIAS recv_shared)