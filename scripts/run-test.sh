#!/usr/bin/env bash

NAME=$(basename $1)

CMOCKA_XML_FILE=$NAME.xml $1
xunit-viewer -r $NAME.xml -o $NAME.html
