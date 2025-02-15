#!/bin/bash



rm -rf /tmp/investAPI
git clone git@github.com:RussianInvestments/investAPI.git /tmp/investAPI



rm -rf libs/investapi/messages
cp -r /tmp/investAPI/src/docs/contracts libs/investapi/messages
rm -rf /tmp/investAPI



mkdir -p libs/investapi/messages/generated
cd libs/investapi/messages

for file in `find -name \*.proto`
do
    sed -i "s/package tinkoff.public.invest.api.contract/package tinkoff.pub.invest.api.contract/g" ${file}
done

for file in `find -name \*.proto`
do
    protoc -I=. --cpp_out=generated --grpc_out=generated --plugin=protoc-gen-grpc="E:\Qt\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe" ${file}
done



cd ../../..
./generate_sources.sh
