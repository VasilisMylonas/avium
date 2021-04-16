#!/usr/bin/env bash

BUILD_DIR=$1
BUILD_TYPE=$2

cmake -E make_directory $BUILD_DIR
cmake -S . -B $BUILD_DIR -DAVM_USE_GC=OFF -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build $BUILD_DIR --target HtmlDocs

mkdir -p ./site
cp $BUILD_DIR/docs/sphinx/*.js ./site
cp $BUILD_DIR/docs/sphinx/*.html ./site
cp -r $BUILD_DIR/docs/sphinx/_static ./site/static
cp -r $BUILD_DIR/docs/sphinx/_sources ./site/sources

for f in ./site/*.html; do
    sed -i 's/_static/static/g' $f
    sed -i 's/_sources/sources/g' $f
done
