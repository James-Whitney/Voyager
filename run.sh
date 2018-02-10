#!/bin/bash

cd build
make
if [[ $? != 0 ]]; then 
  exit
fi

cd ..
if [[ $1 == 0 ]]; then 
./build/bin/voyager-client
else 
./build/bin/voyager-server
fi
