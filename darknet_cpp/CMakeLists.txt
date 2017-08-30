project(darknet_cpp)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH "${darknet_cpp_SOURCE_DIR}/cmake")

# Request C++11 standard, using new CMake variables.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(build_type)

# flags and parameters
set (GPU true CACHE BOOL "Use CUDA")
set (CUDNN true CACHE BOOL "Use libcudnn")
set (ADDRESS_SANITIZER false CACHE BOOL "Enable address sanitizer. NOTE: only works without cuda/cudnn")
set (default_build_type "Release")

# print the build type
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# original darknet root location
set (DARKNET_ROOT "${darknet_cpp_SOURCE_DIR}/../")

# warning related flags
set (CMAKE_CXX_FLAGS "-Wall -Wextra")

# address sanitizer
if (ADDRESS_SANITIZER)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

# GPU specifics
set (GPU_STR "0")
if (${GPU})
    set (GPU_STR "1")
    add_definitions(-DGPU)
    find_package(CUDA REQUIRED)
endif()

set (CUDNN_LIBS "")
set (CUDNN_STR "0")
if (${CUDNN})
    set (CUDNN_STR "1")
    add_definitions(-DCUDNN)
    set (CUDNN_LIBS "-lcudnn")
endif()

find_package(OpenCV REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)
