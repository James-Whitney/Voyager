#!/usr/bin/env bash

if [ -d build ]; then
   rm -rf build
fi
mkdir build
cd build
<<<<<<< HEAD
if [[ "$1" == "fast" ]]; then
  cmake .. && make -j4
else
  cmake .. && make
fi
=======
cmake .. && make -j4

>>>>>>> vfc
