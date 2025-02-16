INCLUDEPATH += \
    $${PWD}/../libs/investapi \
    $${PWD}/../libs/investapi/messages/generated

LIBS += \
    -L$${OUT_PWD}/../libs/investapi/build \
    -linvestapi
