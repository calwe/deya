#!/bin/bash

# generate makefiles and build
premake5 gmake
make clean
time make