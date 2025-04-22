isEmpty(QUAZIP_PATH):QUAZIP_PATH=$$(QUAZIP_PATH)

isEmpty(QUAZIP_PATH) {
    win32* {
        QUAZIP_PATH = E:/Qt/quazip-1.5
    } else {
        QUAZIP_PATH = /home/gris/Qt/quazip-1.5
    }
}

win32* {
    INCLUDEPATH += \
        $${QUAZIP_PATH}/install/include/QuaZip-Qt6-1.5

    CONFIG (debug, debug|release) {
        DEBUG_SUFFIX = d
    } else {
        DEBUG_SUFFIX =
    }

    LIBS += \
        -L$${QUAZIP_PATH}/install/lib \
        -lbzip2$${DEBUG_SUFFIX} \
        -lquazip1-qt6$${DEBUG_SUFFIX}
} else {
    INCLUDEPATH += \
        $${QUAZIP_PATH}/install/include/QuaZip-Qt6-1.5

    CONFIG (debug, debug|release) {
        DEBUG_SUFFIX = d
    } else {
        DEBUG_SUFFIX =
    }

    LIBS += \
        -L$${QUAZIP_PATH}/install/lib \
        -lquazip1-qt6$${DEBUG_SUFFIX}
}
