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



copy_quazip_files.commands = $(COPY_FILE) "$$shell_path($${QUAZIP_PATH}/bin/quazip1-qt6$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)"
first.depends = $(first) copy_quazip_files

export(first.depends)
export(copy_quazip_files.commands)

QMAKE_EXTRA_TARGETS += first copy_quazip_files
