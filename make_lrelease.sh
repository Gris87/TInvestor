#!/bin/bash

cd app

lrelease app.pro

cd ..



cd tools/qt/notifier

lrelease notifier.pro

cd ../../..
