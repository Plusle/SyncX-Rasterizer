cmake_minimum_required(VERSION 3.15.0)

project (SyncXRenderer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(include)

add_subdirectory(src)
# add_executable(${PROJECT_NAME} main.cpp)
