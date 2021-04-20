#!/usr/bin/env bash

# Build the library.
cmake --build ./build --config $1

## Build the documentation.
cmake --build ./build --target HtmlDocs
