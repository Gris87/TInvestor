DEPENDPATH += \
    $${PWD}/../libs/investapi

INCLUDEPATH += \
    $${PWD}/../libs/investapi \
    $${PWD}/../libs/investapi/messages/generated

LIBS += \
    -L$${OUT_PWD}/$${PROJECT_ROOT_PATH}/libs/investapi/build \
    -linvestapi

include(../libs/grpc.pri)
