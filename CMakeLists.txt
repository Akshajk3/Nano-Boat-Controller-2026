cmake_minimum_required(VERSION 3.10)
project(boat-controller)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Vescuart REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(boat-controller ${SOURCES})

target_include_directories(boat-controller PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${VESCUART_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(boat-controller PRIVATE
    ${VESCUART_LIBRARIES}
    Boost::system
    Boost::thread
    Qt6::Core
    Qt6::Widgets
)