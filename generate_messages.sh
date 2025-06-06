#!/bin/bash



if [ "${GRPC_CPP_PLUGIN}" == "" ]; then
    if [ "${MSYSTEM}" != "" ]; then
        GRPC_CPP_PLUGIN="E:\Qt\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe"
    else
        GRPC_CPP_PLUGIN="/home/gris/Qt/vcpkg/installed/x64-linux/tools/grpc/grpc_cpp_plugin"
    fi
fi



rm -rf /tmp/investAPI
rm -rf /tmp/invest-python
git clone git@github.com:RussianInvestments/investAPI.git /tmp/investAPI
git clone git@github.com:RussianInvestments/invest-python.git /tmp/invest-python



rm -rf libs/investapi/messages
cp -r /tmp/investAPI/src/docs/contracts libs/investapi/messages
rm -rf /tmp/investAPI

rm -rf tools/py3/tinkoff-accounts/tinkoff
rm -rf tools/py3/tinkoff-follower/tinkoff
cp -r /tmp/invest-python/tinkoff tools/py3/tinkoff-accounts/tinkoff
cp -r /tmp/invest-python/tinkoff tools/py3/tinkoff-follower/tinkoff
rm -rf /tmp/invest-python



mkdir -p libs/investapi/messages/generated
cd libs/investapi/messages

for file in `find -name \*.proto`
do
    protoc -I=. --cpp_out=generated --grpc_out=generated --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}" ${file}
done

for file in `find -name \*.grpc.pb.\*`
do
    sed -i "s/namespace public/namespace public_/g"   ${file}
    sed -i "s/tinkoff::public::/tinkoff::public_::/g" ${file}
done



cd ../../..
./generate_sources.sh
