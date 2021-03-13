#!/usr/bin/env bash

for TEST in $1/*; do
    NAME=$(basename $TEST)

    if [ $NAME == default.profraw ]; then
        continue
    fi

    LLVM_PROFILE_FILE=$NAME.profraw $TEST >$NAME.tap
    # pytap2 html $NAME.tap >${NAME}-test-results.html

    llvm-profdata-10 merge -sparse $NAME.profraw -o $NAME.profdata
    llvm-cov-10 show --format=html $TEST -instr-profile=$NAME.profdata -o $NAME

    x-www-browser ${NAME}-test-results.html &>/dev/null
    # x-www-browser $NAME/index.html &>/dev/null
done
