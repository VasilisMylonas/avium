#!/usr/bin/env bash

sudo apt update &&
sudo apt install -y cmake doxygen python3 python3-pip lcov &&
sudo pip3 install sphinx breathe sphinx-rtd-theme
