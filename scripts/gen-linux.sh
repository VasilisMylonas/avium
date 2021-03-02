#!/bin/bash

./scripts/build.sh

DIST_DIR="./dist"

if [ -d "$DIST_DIR" ]; then
    rm -rf $DIST_DIR
fi

mkdir -p $DIST_DIR/linux
mkdir -p $DIST_DIR/docs

cp -r ./build/html $DIST_DIR/docs/

cp -r ./build/lib $DIST_DIR/linux/
cp -r ./build/include $DIST_DIR/linux
cp -r ./include $DIST_DIR/linux

rm $DIST_DIR/linux/include/avium/config.h.in
