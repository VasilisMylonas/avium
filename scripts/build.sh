#!/bin/bash

cmake --build ./build
./scripts/gen-docs.sh
./scripts/gen-tests.sh
