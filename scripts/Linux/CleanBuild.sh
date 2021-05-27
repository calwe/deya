#!/bin/bash

cd ../.. # premake file location

cd Deya/vendor/nfd/build/gmake_linux
make config=release_x64
cd ../../../../..

# generate makefiles and build
premake5 --cc=clang gmake
make clean
time make

cd scripts/Linux # back to start location