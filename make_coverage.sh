#!/bin/bash

cd build/Desktop-Debug/test/build

export LD_LIBRARY_PATH=`pwd`
./tests > output.txt 2>&1
RESULT_CODE=$?

if [ ${RESULT_CODE} -eq 0 ]; then
    lcov -t "tests" --capture --directory gen/tests/objs --output-file tests.info

    rm -rf ../../../CoverageReport
    genhtml --output-directory ../../../CoverageReport tests.info

    if [ "%1" != "--ci" ]; then
        ../../../CoverageReport/index.html
    fi
else
    cat output.txt
fi

rm output.txt

exit ${RESULT_CODE}
