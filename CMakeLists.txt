cmake_minimum_required(VERSION 3.23)
project(Tests)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -m64 -Wno-return-type -O2")
set(CMAKE_SKIP_RPATH ON)

add_executable(Tests
	Source/Tests.cpp
	Source/framework/Hooking.h
	Source/framework/PatternScan.h
	Source/framework/ReturnAddr.h
	Source/framework/Assembly.h
	Source/framework/Memory.h
)
