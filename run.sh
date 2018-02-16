#!/bin/bash

cd build
make
cd ..
./build/bin/voyager-client $1
