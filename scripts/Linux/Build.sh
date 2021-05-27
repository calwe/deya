#!/bin/bash

# generate makefiles and build
cd ../.. # premake file location

premake5 --cc=clang gmake

cd Deya/vendor/nfd/build/gmake_linux
make config=release_x64
cd ../../../../..

time make

cd scripts/Linux # back to start location