cmake_minimum_required(VERSION 3.6)
project(PracticeHashCode2017)

set(ENV{Path} C:/cygwin64/bin/)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Pizza.cpp Pizza.h)
add_executable(PracticeHashCode2017 ${SOURCE_FILES})