#!/usr/bin/env bash

BUILD_DIR=$1
BUILD_TYPE=$2

cd $BUILD_DIR
ctest -C $BUILD_TYPE
