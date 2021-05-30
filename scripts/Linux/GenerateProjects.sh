#!/bin/bash

cd ../.. # premake file location

{ # try
    premake5 export-compile-commands
} || { # catch
    echo "'premake5 export-compile-commands' not found, assuming ccls isnt being used"
}
premake5 gmake

cd scripts/Linux # back to start location
