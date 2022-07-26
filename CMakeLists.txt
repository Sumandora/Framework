cmake_minimum_required(VERSION 3.20)
project(Framework)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type -O2")
set(CMAKE_SKIP_RPATH ON)

# Hooking
option(HOOKING_DETOUR "Enables support for detour hooking" ON)
option(HOOKING_PTRSWAP "Enables support for ptr swap hooking" ON)

# Patterns
option(PATTERN_SCANNING "Enables support for pattern scanning" ON)

# Return Address
option(RETURN_ADDRESS "Enables support for return address spoofing" ON)

add_executable(Framework
	Source/Framework/Hooking/Hooking.cpp
	Source/Framework/Memory/Memory.cpp
	Source/Framework/Assembly/Assembly.cpp
	Source/Framework/PatternScan/PatternScan.cpp

	Source/Tests/Main.cpp
)

if(HOOKING_DETOUR)
	target_compile_definitions(Framework PUBLIC FRAMEWORK_ENABLE_HOOKING_DETOUR)
endif()

if(HOOKING_PTRSWAP)
	target_compile_definitions(Framework PUBLIC FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
endif()

if(PATTERN_SCANNING)
	target_compile_definitions(Framework PUBLIC FRAMEWORK_ENABLE_PATTERN_SCANNING)
endif()

if(RETURN_ADDRESS)
	target_compile_definitions(Framework PUBLIC FRAMEWORK_ENABLE_RETURN_ADDRESS)
endif()
