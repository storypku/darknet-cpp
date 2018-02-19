![Darknet Logo](http://pjreddie.com/media/files/darknet-black-small.png)

# Darknet

This is yet another fork of the darknet detection framework with some extra features including:

* C++ interface (inference only)

For more general information on darknet see the [Darknet project website](http://pjreddie.com/darknet).

## Compiling the C++ interface

Requirements:
    * OpenCV 3
    * cmake

```
cd darknet_cpp
mkdir build
cd build
cmake-gui ..
```

You will find similar build options in the config as provided inside the original Makefile of darknet.
Cmake will build the C++ wrapper code and calls the original Makefile to build the darknet C code.
Relevant variables from cmake are passed to the original Makefile which requires no modifications.

Build and install:

```
make
make install
```

Cleaning:

```
make clean      # only cleans the C++ wrapper objects
make cleanall   # also calls clean target from original Makefile
```

## Running provided examples

```
cd darknet_cpp/build
./examples/darknet_cpp_detection ../../cfg/coco.data ../../cfg/yolo.cfg ../../weights/yolo.weights my_video.mp4
./examples/darknet_cpp_detection_threaded ../../cfg/coco.data ../../cfg/yolo.cfg ../../weights/yolo.weights my_video.mp4
```

## Using the C++ interface

See example source code 'darknet_cpp/examples'

