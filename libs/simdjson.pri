DEPENDPATH += \
    $${PWD}/../libs/simdjson

INCLUDEPATH += \
    $${PWD}/../libs/simdjson

LIBS += \
    -L$${OUT_PWD}/../libs/simdjson/build \
    -lsimdjson
