cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE includes *.h)

add_library(TerminalLowLevel STATIC ${sources} ${includes})
target_include_directories(TerminalLowLevel PUBLIC ./)
