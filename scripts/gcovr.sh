#!/usr/bin/env bash

GCDA_TEST_FILES=$(find tests/CMakeFiles/*.dir/*.gcda)
GCDA_FILES=$(find src/CMakeFiles/*.dir/*.gcda)

geninfo $GCDA_FILES $GCDA_TEST_FILES -b . -o coverage/lcov.info
# genhtml ../coverage/coverage.info -o ../coverage
