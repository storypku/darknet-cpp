#
# Author: Maarten Vandersteegen
# Darknet C++
# TODO: add header dependencies, build objects in build directory
#

GPU ?= 1
CUDNN ?= 1
DEBUG ?= 0
PREFIX ?= /usr/local

LIBNAME = libdarknet_cpp
LIBDARKNET = ../libdarknet

CXX = g++
# for c++ linking
CC = $(CXX)

# TODO: add address sanitizer in debug mode
ifeq ($(DEBUG), 1)
CXXFLAGS += -g -O0
else
CXXFLAGS += -Ofast
endif

CXXFLAGS += -std=c++11 -fPIC

COMMON += -DCUDNN -DGPU
CXXFLAGS += $(COMMON)
LDFLAGS += $(COMMON)

# darknet
CXXFLAGS += -I../src/ -I../include -Isrc

# opencv
LDLIBS += $(shell pkg-config --libs opencv)
CXXFLAGS += $(shell pkg-config --cflags opencv)

# boost
LDLIBS += -lboost_filesystem -lboost_system -pthread

ifeq ($(GPU), 1)
# cuda
CXXFLAGS += -I/usr/local/cuda/include/
LDLIBS += -L/usr/local/cuda/lib64 -lcuda -lcuda -lcudart -lcublas -lcurand
endif

ifeq ($(CUDNN), 1)
# cudnn
LDLIBS += -lcudnn
endif

# export variables for darknet makefile
OPENCV=0
export GPU CUDNN OPENCV DEBUG

# object files from c++ interface to construct the library
LIBSRC = $(wildcard src/*.cpp)
LIBOBJ = $(LIBSRC:.cpp=.o)

###########################################################
#
# Targets go here
#
###########################################################

all: $(LIBNAME).so \
        examples/darknet_cpp_jetson_detection

# example targets
examples/darknet_cpp_jetson_detection: examples/darknet_cpp_jetson_detection.o $(LIBOBJ) $(LIBDARKNET).a

# install target
install: $(LIBNAME).so $(LIBNAME).pc
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/lib/pkgconfig
	install -d $(PREFIX)/include/darknet_cpp
	install $(LIBNAME).so $(PREFIX)/lib
	install -m 644 $(LIBNAME).pc $(PREFIX)/lib/pkgconfig
	install -m 644 src/*.hpp $(PREFIX)/include/darknet_cpp

$(LIBNAME).pc:
	echo 'prefix=$(PREFIX)' > $(LIBNAME).pc
	cat $(LIBNAME).pc.make >> $(LIBNAME).pc

# shared library target
$(LIBNAME).so: $(LIBOBJ) $(LIBDARKNET).a
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(LIBDARKNET).a:
	make -C ../

clean:
	rm -rf src/*.o examples/*.o $(LIBNAME).a $(LIBNAME).so $(LIBNAME).pc \
        examples/darknet_cpp_jetson_detection

cleanall: clean
	make -C ../ clean

.PHONY: clean cleanall all install
