include (${CMAKE_ROOT}/Modules/ExternalProject.cmake)

include_directories (${OpenCV_INCLUDE_DIRS})
include_directories ("${DARKNET_ROOT}/include")

set (HEADERS
    ${DARKNET_ROOT}/include/darknet.h
    darknet.hpp
    detection.hpp
    detector.hpp
    image.hpp
    logging.hpp
    utils.hpp
)

set (SOURCES
    detector.cpp
    image.cpp
    utils.cpp
)

set (SRC ${SOURCES} ${HEADERS})

set (DEBUG_STR "0")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUG_STR "1")
endif()

# original darknet library
add_custom_target (
    darknet
    COMMAND make GPU=${GPU_STR} CUDNN=${CUDNN_STR} OPENCV=0 DEBUG=${DEBUG_STR}
    WORKING_DIRECTORY ${DARKNET_ROOT}
)

add_library (darknet_cpp_static STATIC ${SRC})
add_library (darknet_cpp SHARED ${SRC})

add_dependencies (darknet_cpp darknet)
target_link_libraries (darknet_cpp
    "-lboost_filesystem -lboost_system"
    ${DARKNET_ROOT}/libdarknet.a
    "-pthread"
    ${OpenCV_LIBS}
    ${GPU_LIBS}
)

# install library target and header files
install (TARGETS darknet_cpp
    DESTINATION "lib"
)

install (FILES ${HEADERS}
    DESTINATION "include/darknet_cpp"
)
