#!/bin/bash

# generate makefiles and build
premake5 gmake
make clean
make

# ask if user would like to run generated project
read -r -p "would you like to run the generated file? [Y/n] " input
 
case $input in
    [yY][eE][sS]|[yY])
 ./bin/Debug-linux-x86_64/Sandbox/Sandbox
 ;;
    [nN][oO]|[nN])
       ;;
    *)
 echo "Invalid input..."
 exit 1
 ;;
esac