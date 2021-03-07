#!/bin/bash

TEST_DIR="./build/test"

mkdir -p $TEST_DIR

./build/bin/test --avm-tap-output=$TEST_DIR/results.tap

cat $TEST_DIR/results.tap | tapview
