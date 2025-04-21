#!/bin/bash

cd build/Desktop-Debug/test/build

export LD_LIBRARY_PATH=`pwd`
./tests > output.txt 2>&1
RESULT_CODE=$?

if [ ${RESULT_CODE} -eq 0 ]; then
    lcov \
        -t "tests" \
        --capture \
        --include app/src/* \
        --exclude */test_* \
        --exclude */messagebox.cpp \
        --exclude */httpclient.cpp \
        --directory gen/tests/objs \
        --output-file tests.info

    rm -rf ../../../CoverageReport
    genhtml --output-directory ../../../CoverageReport tests.info

    if [ "$1" != "--ci" ]; then
        firefox ../../../CoverageReport/index.html > /dev/null 2>&1 &
    fi
else
    cat output.txt
fi

rm output.txt

exit ${RESULT_CODE}
