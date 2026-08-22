
cmake_minimum_required(VERSION 3.16)
project(Arbiter LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
 
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
 
add_executable(engine
    src/arbiter.cpp
    src/uci.cpp
    src/search.cpp
    src/eval.cpp
)
 
target_include_directories(engine PRIVATE
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/include
)