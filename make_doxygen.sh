#!/bin/bash -ex

mkdir -p build

doxygen

if [ "$1" != "--ci" ]; then
    firefox build/doxygen/html/index.html > /dev/null 2>&1 &
fi
