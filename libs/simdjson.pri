DEPENDPATH += \
    $${PWD}/../libs/simdjson

INCLUDEPATH += \
    $${PWD}/../libs/simdjson

LIBS += \
    -L$${OUT_PWD}/$${PROJECT_ROOT_PATH}/libs/simdjson/build \
    -lsimdjson
