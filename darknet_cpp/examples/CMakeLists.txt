include_directories ("${DARKNET_ROOT}/include")
include_directories (${darknet_cpp_SOURCE_DIR}/src)
link_directories (${darknet_cpp_SOURCE_DIR}/src)

add_executable(darknet_cpp_jetson_detection darknet_cpp_jetson_detection.cpp)

# examples link statically for now
target_link_libraries(darknet_cpp_jetson_detection
    darknet_cpp_static
    "-lboost_filesystem -lboost_system"
    "-lpthread"
    ${DARKNET_ROOT}/libdarknet.a
    ${OpenCV_LIBS}
    ${GPU_LIBS}
)
add_dependencies (darknet_cpp_jetson_detection darknet)
