cmake_minimum_required(VERSION 3.10)
project(boat-controller)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Vescuart REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(boat-controller ${SOURCES})

target_include_directories(boat-controller PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${VESCUART_INCLUDE_DIRS}
)

target_link_libraries(boat-controller
    ${VESCUART_LIBRARIES})