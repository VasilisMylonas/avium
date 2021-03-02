#!/bin/bash

cmake --build ./build
mkdir -p ./build/html
natural-docs ./docs/
