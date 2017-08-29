project(darknet_cpp)
cmake_minimum_required(VERSION 3.5)

# flags and parameters
set (GPU true CACHE BOOL "Use CUDA")
set (CUDNN true CACHE BOOL "Use libcudnn")
set (PREFIX "/usr/local")

# print the build type
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# original darknet root location
set (DARKNET_ROOT "${darknet_cpp_SOURCE_DIR}/../")

# general compiler flags
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")

# debug specific define for logging output
set (CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")

# GPU specifics
set (GPU_LIBS "")

if (${GPU})
    add_definitions(-DGPU)
    set (GPU_LIBS ${GPU_LIBS} "-L/usr/local/cuda/lib64 -lcuda -lcuda -lcuda -lcudart -lcublas -lcurand")
    include_directories("/usr/local/cuda/include/")
endif()

if (${CUDNN})
    add_definitions(-DCUDNN)
    set (GPU_LIBS ${GPU_LIBS} "-lcudnn")
endif()

find_package(OpenCV REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)
