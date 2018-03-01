#!/bin/bash

cd build
make
if [[ $? != 0 ]]; then 
  exit
fi
<<<<<<< HEAD

cd ..
if [[ $1 == 0 ]]; then 
./build/bin/voyager-client
else 
./build/bin/voyager-server
fi
=======
cd ..
./build/bin/voyager-client $1
>>>>>>> vfc
