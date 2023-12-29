cmake_minimum_required(VERSION 3.14)

project(plane-sweep)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME} ${sources})
