#!/usr/bin/env bash

GCDA_TEST_FILES=$(find ./CMakeFiles/*.dir/*.gcda)
GCDA_FILES=$(find ../src/CMakeFiles/*.dir/*.gcda)

geninfo $GCDA_FILES $GCDA_TEST_FILES -b ../../src -o ./coverage.info
genhtml ./coverage.info -o ../coverage
