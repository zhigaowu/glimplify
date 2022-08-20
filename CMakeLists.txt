cmake_minimum_required(VERSION 3.20)

project(glimplify CXX C)

# add_definitions(-DFOO -DDEBUG ...)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

include_directories(
    $ENV{GLAD_PATH}/include
    $ENV{GLFW_PATH}/include
    $ENV{GLM_PATH}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

link_directories(
    $ENV{GLFW_PATH}/lib
)

file(GLOB GLIMPLIFY_SOURCES_HPP ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB GLIMPLIFY_SOURCES_CHH ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${GLIMPLIFY_SOURCES_CHH} ${GLIMPLIFY_SOURCES_HPP} $ENV{GLAD_PATH}/src/glad.c)
target_link_libraries(${PROJECT_NAME} glfw3)

