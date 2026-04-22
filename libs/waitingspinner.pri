DEPENDPATH += \
    $${PWD}/../libs/waitingspinner

INCLUDEPATH += \
    $${PWD}/../libs/waitingspinner

LIBS += \
    -L$${OUT_PWD}/$${PROJECT_ROOT_PATH}/libs/waitingspinner/build \
    -lwaitingspinner
