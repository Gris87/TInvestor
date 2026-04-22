DEPENDPATH += \
    $${PWD}/../libs/simplecrypt

INCLUDEPATH += \
    $${PWD}/../libs/simplecrypt

LIBS += \
    -L$${OUT_PWD}/$${PROJECT_ROOT_PATH}/libs/simplecrypt/build \
    -lsimplecrypt
