#!/bin/bash

cd ../.. # premake file location

# generate makefiles and build
premake5 gmake
make clean
time make

cd BuildScripts/Linux # back to start location