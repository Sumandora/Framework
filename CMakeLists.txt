cmake_minimum_required(VERSION 3.23)
project(Tests)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -m64 -Wno-return-type")
set(CMAKE_SKIP_RPATH ON)

add_executable(Tests Source/Tests.cpp Source/framework/Framework.h Source/framework/hooking/Hooking.h Source/framework/patternscan/PatternScan.h Source/framework/returnaddr/ReturnAddr.h Source/framework/assembly/Assembly.h Source/framework/memory/Memory.h)
