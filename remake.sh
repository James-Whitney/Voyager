#!/usr/bin/env bash

if [ -d build ]; then
   rm -rf build
fi
mkdir build
cd build
if [[ "$1" == "fast" ]]; then
  cmake .. && make -j4
else
  cmake .. && make
fi
