DEPENDPATH += \
    $${PWD}/../libs/simplecrypt

INCLUDEPATH += \
    $${PWD}/../libs/simplecrypt

LIBS += \
    -L$${OUT_PWD}/../libs/simplecrypt/build \
    -lsimplecrypt
