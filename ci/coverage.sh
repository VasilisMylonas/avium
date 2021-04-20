#!/usr/bin/env bash

# Find the .gcda files.
GCDA_TEST_FILES=$(find ./build/tests/CMakeFiles/*.dir/*.gcda)
GCDA_SRC_FILES=$(find ./build/src/CMakeFiles/*.dir/*.gcda)

mkdir -p ./build/coverage

# Generate the info file.
geninfo $GCDA_SRC_FILES $GCDA_TEST_FILES -o $1
