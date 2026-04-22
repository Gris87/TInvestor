#!/bin/bash



cd build/Desktop-Debug/tests/app_tests/build

export LD_LIBRARY_PATH=`pwd`
./app_tests > output.txt 2>&1
RESULT_CODE=$?

if [ ${RESULT_CODE} -ne 0 ]; then
    cat output.txt
    rm output.txt

    exit ${RESULT_CODE}
fi

rm output.txt

cd ../../../../../



cd build/Desktop-Debug/tests/notifier_tests/build

export LD_LIBRARY_PATH=`pwd`
./notifier_tests > output.txt 2>&1
RESULT_CODE=$?

if [ ${RESULT_CODE} -ne 0 ]; then
    cat output.txt
    rm output.txt

    exit ${RESULT_CODE}
fi

rm output.txt

cd ../../../../../



rm -rf build/CoverageReport



cd build/Desktop-Debug/tests/app_tests/build

lcov \
    -t "app_tests" \
    --capture \
    --include app/src/* \
    --exclude */test_* \
    --exclude */*_mock.h \
    --exclude */filedialog.cpp \
    --exclude */messageboxutils.cpp \
    --exclude */httpclient.cpp \
    --directory gen/app_tests/objs \
    --output-file lcov.info
RESULT_CODE=$?

if [ ${RESULT_CODE} -eq 0 ]; then
    genhtml --output-directory ../../../../CoverageReport/app_tests lcov.info
    RESULT_CODE=$?

    mv lcov.info ../../../../CoverageReport/app_tests

    if [ "$1" != "--ci" ]; then
        firefox ../../../../CoverageReport/app_tests/index.html > /dev/null 2>&1 &
    fi
fi

cd ../../../../../



cd build/Desktop-Debug/tests/notifier_tests/build

lcov \
    -t "notifier_tests" \
    --capture \
    --include notifier/src/* \
    --exclude */test_* \
    --exclude */*_mock.h \
    --directory gen/notifier_tests/objs \
    --output-file lcov.info
RESULT_CODE=$?

if [ ${RESULT_CODE} -eq 0 ]; then
    genhtml --output-directory ../../../../CoverageReport/notifier_tests lcov.info
    RESULT_CODE=$?

    mv lcov.info ../../../../CoverageReport/notifier_tests

    if [ "$1" != "--ci" ]; then
        firefox ../../../../CoverageReport/notifier_tests/index.html > /dev/null 2>&1 &
    fi
fi

cd ../../../../../
