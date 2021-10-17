cmake_minimum_required(VERSION 3.15.0)

project (SyncXRasterizer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_subdirectory(src)

# option(MIPMAP_TEST "Save compaticities of each layer" ON)

# add_executable(${PROJECT_NAME} main.cpp)
 