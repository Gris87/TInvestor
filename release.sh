#!/bin/bash

VERSION=${1}

if [[ ! "${VERSION}" =~ ^v[0-9]+\.[0-9]+.[0-9]+$ ]]; then
    echo "Provide correct version!"

    exit 1
fi

sed -i "s/VERSION *= *\".*\"/VERSION = \"${VERSION}\"/g" app/src/main.cpp

git tag -a ${VERSION} -m "Release ${VERSION}" || exit 1
./commit.sh "Release ${VERSION}"
