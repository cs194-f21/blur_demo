#!/bin/bash

rm -rf build
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
cmake --build build

## depends on GNU parallel (NOT from moreutils)
##
## $ sudo apt install parallel

# use 50% to avoid hyperthreading
parallel -j 50% ::: ./build/blur_* | sort -n -k11 | tail

