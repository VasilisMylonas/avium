#!/usr/bin/env bash

GCDA_TEST_FILES=$(find ./CMakeFiles/*.dir/*.gcda)
GCDA_FILES=$(find ../src/CMakeFiles/*.dir/*.gcda)

mkdir -p ../coverage
geninfo $GCDA_FILES $GCDA_TEST_FILES -b ../../src -o ../coverage/coverage.info
genhtml ../coverage/coverage.info -o ../coverage
