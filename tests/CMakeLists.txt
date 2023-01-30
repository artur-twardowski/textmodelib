cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE includes *.h)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test ${sources} ${includes})
target_link_libraries(test PUBLIC TerminalLowLevel)
target_link_libraries(test PUBLIC gtest)

