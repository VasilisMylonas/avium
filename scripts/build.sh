#!/usr/bin/env bash

BUILD_DIR=$1
BUILD_TYPE=$2

cmake -E make_directory $BUILD_DIR
cmake -S . -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build $BUILD_DIR --config $BUILD_TYPE
