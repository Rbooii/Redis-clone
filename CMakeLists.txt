cmake_minimum_required(VERSION 3.20)
project(RedisClone C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Active project — pure C
add_executable(redis-clone
    src/main.cpp
    src/utils/CoreDebug.cpp
)
target_include_directories(redis-clone PRIVATE src src/utils)

#server_dev
add_executable(redis-server
    src/server.cpp
    src/utils/CoreDebug.cpp
    src/utils/CoreIO.cpp
)
target_include_directories(redis-server PRIVATE src src/utils)

#client_dev
add_executable(redis-client
    src/client.cpp
    src/utils/CoreDebug.cpp
    src/utils/CoreIO.cpp
)
target_include_directories(redis-client PRIVATE src src/utils)

# Legacy C++ prototype, kept for reference, excluded from default build
add_executable(legacy-server-test EXCLUDE_FROM_ALL
    legacy_cpp/server_test.cpp
)
target_include_directories(legacy-server-test PRIVATE legacy_cpp)