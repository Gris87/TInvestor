#!/bin/bash

mkdir build

doxygen
firefox build/doxygen/html/index.html > /dev/null 2>&1 &
