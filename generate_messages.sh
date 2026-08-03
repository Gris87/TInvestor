#!/bin/bash



if [ "${GRPC_CPP_PLUGIN}" == "" ]; then
    if [ "${MSYSTEM}" != "" ]; then
        GRPC_CPP_PLUGIN="E:\Qt\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe"
    else
        GRPC_CPP_PLUGIN="/home/gris/Qt/vcpkg/installed/x64-linux/tools/grpc/grpc_cpp_plugin"
    fi
fi



rm -rf /tmp/invest-contracts
rm -rf /tmp/invest-python
git clone https://opensource.tbank.ru/invest/invest-contracts.git /tmp/invest-contracts
git clone https://opensource.tbank.ru/invest/invest-python.git /tmp/invest-python



rm -rf libs/investapi/messages
cp -r /tmp/invest-contracts/src/docs/contracts libs/investapi/messages
rm -rf /tmp/invest-contracts

rm -rf tools/py3/tinkoff-accounts/t_tech
rm -rf tools/py3/tinkoff-annihilate-mm/t_tech
rm -rf tools/py3/tinkoff-asap-trading/t_tech
rm -rf tools/py3/tinkoff-bidir-trading/t_tech
rm -rf tools/py3/tinkoff-fall-trading/t_tech
rm -rf tools/py3/tinkoff-favorite-group/t_tech
rm -rf tools/py3/tinkoff-follower/t_tech
rm -rf tools/py3/tinkoff-terminate-trading/t_tech
rm -rf tools/py3/tinkoff-tmon-weekend-trading/t_tech
rm -rf tools/py3/tinvestor-notifier/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-accounts/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-annihilate-mm/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-asap-trading/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-bidir-trading/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-fall-trading/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-favorite-group/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-follower/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-terminate-trading/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinkoff-tmon-weekend-trading/t_tech
cp -r /tmp/invest-python/t_tech tools/py3/tinvestor-notifier/t_tech
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
