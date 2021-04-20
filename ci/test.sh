#!/usr/bin/env bash

# Run the tests
cd ./build && ctest -C $1
