#!/bin/bash

cd $(git rev-parse --show-toplevel)

cd cpp/externals/googletest
mkdir -p build && cd build && cmake ..
make clean && make -j6
cd ../..

cd glog
mkdir -p build && cd build && cmake ..
make clean && make -j6

