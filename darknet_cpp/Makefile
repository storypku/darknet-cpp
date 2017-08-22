CXX = g++
CC = $(CXX)

ifeq ($(DEBUG), 1)
CXXFLAGS += -g -O0
endif

CXXFLAGS += -std=c++11

COMMON += -DCUDNN -DGPU
CXXFLAGS += $(COMMON)
LDFLAGS += $(COMMON)

# darknet
CXXFLAGS += -I../src/ -I../include
LDLIBS += -L.. -ldarknet

# opencv
LDLIBS += $(shell pkg-config --libs opencv)
CXXFLAGS += $(shell pkg-config --cflags opencv)

# boost
LDLIBS += -lboost_filesystem -lboost_system

# cuda
CXXFLAGS += -I/usr/local/cuda/include/
LDLIBS += -L/usr/local/cuda/lib64 -lcuda -lcuda -lcudart -lcublas -lcurand

test: test.o detector.o image_overlay.o image.o

clean:
	rm -rf test *.o
