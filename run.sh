#!/bin/bash

cd build
make
cd ..
if [[ $1 == 0 ]]; then 
./build/bin/voyager-client
else 
./build/bin/voyager-server
fi
