#!/bin/bash

cd ../.. # premake file location

# generate makefiles and build
premake5 gmake
make clean
time make

cd scripts/Linux # back to start location