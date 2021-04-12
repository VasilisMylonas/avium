#!/usr/bin/env bash

BUILD_DIR=$1
COVERAGE_FILE=$2

GCDA_TEST_FILES=$(find $BUILD_DIR/tests/CMakeFiles/*.dir/*.gcda)
GCDA_FILES=$(find $BUILD_DIR/src/CMakeFiles/*.dir/*.gcda)

geninfo $GCDA_FILES $GCDA_TEST_FILES -o $COVERAGE_FILE
