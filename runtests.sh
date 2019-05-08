#!/bin/bash

olddir=$(pwd)
cd "$(dirname "$0")"

if [[ "$OSTYPE" == "darwin"* ]]; then
    tests="./build/tests/Debug/tefri-tests"
else
    tests="./build/tests/tefri-tests"
fi


if [[ "$OSTYPE" == "darwin"* ]]; then
    if [ ! -f $tests ]; then
        ./rebuild.sh
    fi
else
    if [ ! -f $tests ]; then
        ./rebuild.sh
    fi
fi

./build.sh
$tests

cd $olddir
