#!/bin/bash

# generate makefiles and build
cd ../.. # premake file location

premake5 gmake
time make

cd scripts/Linux # back to start location