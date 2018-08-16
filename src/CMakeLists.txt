include_directories (${OpenCV_INCLUDE_DIRS})
include_directories (${CUDA_INCLUDE_DIRS})
include_directories ("${DARKNET_ROOT}/include")

set (HEADERS
    convert_cv.hpp
    convert_cv_bgr8.hpp
    darknet.hpp
    detection.hpp
    detector.hpp
    image.hpp
    logging.hpp
    utils.hpp
)

set (SOURCES
    convert_cv_bgr8.cpp
    detector.cpp
    image.cpp
    utils.cpp
)

set (SRC ${SOURCES} ${HEADERS})

add_custom_target (
    cleanall
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND make -C ${DARKNET_ROOT} clean
)

add_library (darknet_cpp_static STATIC ${SRC})
add_library (darknet_cpp SHARED ${SRC})

add_dependencies (darknet_cpp darknet_orig)
target_link_libraries (darknet_cpp
    "-lboost_filesystem -lboost_system"
    ${DARKNET_ROOT}/libdarknet.a
    "-pthread"
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_CUBLAS_LIBRARIES}
    ${CUDA_curand_LIBRARY}
    ${CUDNN_LIBS}
)

# install library target and header files
install (TARGETS darknet_cpp
    DESTINATION "lib"
)

install (FILES ${HEADERS}
    DESTINATION "include/darknet_cpp"
)

install (FILES ${DARKNET_ROOT}/libdarknet.so
    DESTINATION "lib"
)


set (ORIG_HEADERS ${DARKNET_ROOT}/include/darknet.h)
install (FILES ${ORIG_HEADERS}
    DESTINATION "include"
)
