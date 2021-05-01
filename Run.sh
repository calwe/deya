#!/bin/bash

# generate makefiles and build
premake5 gmake
time make

./bin/Debug-linux-x86_64/Sandbox/Sandbox