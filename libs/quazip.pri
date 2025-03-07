QUAZIP_PATH = E:/Qt/quazip-1.4/install

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
