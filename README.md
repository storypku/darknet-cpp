## How to Install

```sh
mkdir build && cd build
cmake ..
make -j 8
sudo make install
```

## Origin

- https://gitlab.com/EAVISE/darknet.git

## What was adjusted from storypku

- CMakeLists.txt for C/C++ shared libraries: libdarknet.so && libdarknet_cpp.so  respectively
- Python setup:  Packaging Python's binding for libdarknet.so darknet.py
