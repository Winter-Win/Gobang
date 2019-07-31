cmake_minimum_required(VERSION 3.7)
project(game_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

find_package(Boost COMPONENTS system REQUIRED)
include_directories(
        "../../include"
        "../../third/msgpack/include"
)

add_executable(game_server main.cpp)
target_link_libraries(game_server ${Boost_LIBRARIES})
