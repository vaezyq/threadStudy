cmake_minimum_required(VERSION 3.21)
project(threadStudy)

set(CMAKE_CXX_STANDARD 17)

#add_compile_options()
add_link_options(-lpthread)

add_executable(threadStudy  deadlock.cpp Chapter3/uniqueLock.cpp)
