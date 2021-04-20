#!/usr/bin/env bash

# Create the build directory.
cmake -E make_directory ./build &&

# Generate the build system.
cmake -S . -B ./build $2 -DCMAKE_BUILD_TYPE=$1
