#!/usr/bin/env bash

# Update packages.
sudo apt update &&

# Install dependencies.
sudo apt install -y cmake doxygen python3 python3-pip lcov libgc1c2 libgc-dev &&

# Install python packages.
sudo pip3 install sphinx breathe sphinx-rtd-theme &&
