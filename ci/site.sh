#!/usr/bin/env bash

# Create the site directory.
mkdir -p ./site &&

# Copy the html and js files.
cp ./build/docs/sphinx/*.js ./site &&
cp ./build/docs/sphinx/*.html ./site &&

# Copy the static and sources directories.
cp -r ./build/docs/sphinx/_static ./site/static &&
cp -r ./build/docs/sphinx/_sources ./site/sources

# Rename old refereneces.
for f in ./site/*.html; do
    sed -i 's/_static/static/g' $f
    sed -i 's/_sources/sources/g' $f
done
