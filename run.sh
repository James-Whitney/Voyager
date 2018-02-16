#!/bin/bash

cd build
make
if [[ $? != 0 ]]; then 
  exit
fi
cd ..
./build/bin/voyager-client $1
