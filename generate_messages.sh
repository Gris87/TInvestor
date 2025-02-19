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
    protoc -I=. --cpp_out=generated --grpc_out=generated --plugin=protoc-gen-grpc="E:\Qt\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe" ${file}
done

for file in `find -name \*.grpc.pb.\*`
do
    sed -i "s/namespace public/namespace public_/g" ${file}
    sed -i "s/tinkoff::public::/tinkoff::public_::/g"   ${file}
done



cd ../../..
./generate_sources.sh
