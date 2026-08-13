cmake_minimum_required(VERSION 3.20)
project(RedisClone)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(redis-clone
    src/main.cpp
)
target_include_directories(redis-clone PRIVATE src)

add_executable(server-test
    src/server_test.cpp
)
target_include_directories(server-test PRIVATE src)