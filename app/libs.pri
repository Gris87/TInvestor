DEPENDPATH += \
    $${PWD}/../libs/investapi \
    $${PWD}/../libs/simplecrypt

INCLUDEPATH += \
    $${PWD}/../libs/investapi \
    $${PWD}/../libs/investapi/messages/generated \
    $${PWD}/../libs/simplecrypt

LIBS += \
    -L$${OUT_PWD}/../libs/investapi/build \
    -L$${OUT_PWD}/../libs/simplecrypt/build \
    -linvestapi \
    -lsimplecrypt

include(../libs/investapi/grpc.pri)
