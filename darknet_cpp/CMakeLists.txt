project(darknet_cpp)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH "${darknet_cpp_SOURCE_DIR}/cmake")

# Request C++11 standard, using new CMake variables.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(build_type)

# flags and parameters
set (WITH_GPU true CACHE BOOL "Use CUDA")
set (WITH_CUDNN true CACHE BOOL "Use libcudnn")
set (WITH_ADDRESS_SANITIZER false CACHE BOOL "Enable address sanitizer. NOTE: only works without cuda/cudnn")
set (default_build_type "Release")

# print the build type
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# original darknet root location
set (DARKNET_ROOT "${darknet_cpp_SOURCE_DIR}/../")

# warning related flags
set (CMAKE_CXX_FLAGS "-Wall -Wextra")

# address sanitizer
set (WITH_ADDRESS_SANITIZER_STR "0")
if (WITH_ADDRESS_SANITIZER)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set (WITH_ADDRESS_SANITIZER_STR "1")
endif()

# GPU specifics
set (WITH_GPU_STR "0")
if (${WITH_GPU})
    set (WITH_GPU_STR "1")
    add_definitions(-DGPU)
    find_package(CUDA REQUIRED)
    include(select_compute_arch)
    CUDA_SELECT_NVCC_ARCH_FLAGS(DETECTED_COMPUTE_ARCH_FLAGS)
    set (COMPUTE_ARCH_FLAGS ${DETECTED_COMPUTE_ARCH_FLAGS} CACHE STRING "Compute capability flags to compile for")
endif()

set (CUDNN_LIBS "")
set (WITH_CUDNN_STR "0")
if (${WITH_CUDNN})
    set (WITH_CUDNN_STR "1")
    add_definitions(-DCUDNN)
    set (CUDNN_LIBS "-lcudnn")
endif()

find_package(OpenCV REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)
