#!/bin/bash



rm -rf /tmp/investAPI
git clone git@github.com:RussianInvestments/investAPI.git /tmp/investAPI



rm -rf app/messages
cp -r /tmp/investAPI/src/docs/contracts app/messages
rm -rf /tmp/investAPI



mkdir -p app/messages/generated
cd app/messages

for file in `find -name \*.proto`
do
    sed -i "s/package tinkoff.public.invest.api.contract/package tinkoff.pub.invest.api.contract/g" ${file}
done

for file in `find -name \*.proto`
do
    protoc -I=. --cpp_out=generated --grpc_out=generated --plugin=protoc-gen-grpc="E:\Qt\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe" ${file}
done



for file in `find generated -name \*.cc`
do
    mv ${file} ${file}.cpp
done



cd ../..
./generate_sources.sh
