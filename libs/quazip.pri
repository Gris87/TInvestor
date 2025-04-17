isEmpty(QUAZIP_PATH):QUAZIP_PATH=$$(QUAZIP_PATH)

isEmpty(QUAZIP_PATH) {
    win32-msvc* {
        QUAZIP_PATH = E:/Qt/quazip-1.4/install
    } else {
        QUAZIP_PATH = /home/gris/Qt/quazip-1.5/install
    }
}

win32-msvc* {
    INCLUDEPATH += \
        $${QUAZIP_PATH}/include/QuaZip-Qt6-1.4

    CONFIG (debug, debug|release) {
        DEBUG_SUFFIX = d
    } else {
        DEBUG_SUFFIX =
    }

    LIBS += \
        -L$${QUAZIP_PATH}/lib \
        -lbzip2$${DEBUG_SUFFIX} \
        -lquazip1-qt6$${DEBUG_SUFFIX}
} else {
    INCLUDEPATH += \
        $${QUAZIP_PATH}/include/QuaZip-Qt6-1.5

    CONFIG (debug, debug|release) {
        DEBUG_SUFFIX = d
    } else {
        DEBUG_SUFFIX =
    }

    LIBS += \
        -L$${QUAZIP_PATH}/lib \
        -lquazip1-qt6$${DEBUG_SUFFIX}
}
