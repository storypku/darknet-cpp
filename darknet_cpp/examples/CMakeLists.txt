include_directories ("${DARKNET_ROOT}/include")
include_directories (${CUDA_INCLUDE_DIRS})
include_directories (${darknet_cpp_SOURCE_DIR}/src)
link_directories (${darknet_cpp_SOURCE_DIR}/src)

add_executable(darknet_cpp_detection darknet_cpp_detection.cpp)
add_executable(darknet_cpp_detection_threaded darknet_cpp_detection_threaded.cpp)
add_executable(darknet_cpp_jetson_detection darknet_cpp_jetson_detection.cpp)

# examples link statically for now
target_link_libraries(darknet_cpp_jetson_detection
    darknet_cpp_static
    "-lboost_filesystem -lboost_system"
    ${DARKNET_ROOT}/libdarknet.a
    "-lpthread"
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_CUBLAS_LIBRARIES}
    ${CUDA_curand_LIBRARY}
    ${CUDNN_LIBS}
)
add_dependencies (darknet_cpp_jetson_detection darknet)

target_link_libraries(darknet_cpp_detection
    darknet_cpp_static
    "-lboost_filesystem -lboost_system"
    ${DARKNET_ROOT}/libdarknet.a
    "-lpthread"
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_CUBLAS_LIBRARIES}
    ${CUDA_curand_LIBRARY}
    ${CUDNN_LIBS}
)
add_dependencies (darknet_cpp_detection darknet)

target_link_libraries(darknet_cpp_detection_threaded
    darknet_cpp_static
    "-lboost_filesystem -lboost_system"
    ${DARKNET_ROOT}/libdarknet.a
    "-lpthread"
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_CUBLAS_LIBRARIES}
    ${CUDA_curand_LIBRARY}
    ${CUDNN_LIBS}
)
add_dependencies (darknet_cpp_detection_threaded darknet)
