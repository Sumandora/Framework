cmake_minimum_required(VERSION 3.23)
project(Cheat)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -m64 -O2")
set(CMAKE_SKIP_RPATH ON)

#add_library(Cheat Source/libCheat.h Source/retaddr/RetAddr.cpp)
add_executable(Cheat Source/Tests.cpp Source/framework/Framework.h Source/framework/hooking/Hooking.h Source/framework/patternscan/PatternScan.h Source/framework/returnaddr/ReturnAddr.h Source/framework/assembly/Assembly.h Source/framework/memory/Memory.h)
target_link_libraries(Cheat dl)