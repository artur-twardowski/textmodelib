cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE includes *.h)

add_executable(demo1 ${sources} ${includes})
target_link_libraries(demo1 PUBLIC TerminalLowLevel)

