#!/bin/bash

cd app

lupdate -no-obsolete app.pro

cd ..



cd tools/qt/notifier

lupdate -no-obsolete notifier.pro

cd ../../..
