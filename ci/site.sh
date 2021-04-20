#!/usr/bin/env bash

# Create the site directory.
mkdir -p ./site &&

# Copy the html and js files.
cp ./build/docs/html/*.js ./site &&
cp ./build/docs/html/*.html ./site &&

# Copy the static and sources directories.
cp -r ./build/docs/html/_static ./site/static &&
cp -r ./build/docs/html/_sources ./site/sources

# Rename old refereneces.
for f in ./site/*.html; do
    sed -i 's/_static/static/g' $f
    sed -i 's/_sources/sources/g' $f
done
