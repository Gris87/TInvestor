include(../libs/grpc.pri)
include(../libs/investapi.pri)
include(../libs/quazip.pri)
include(../libs/simplecrypt.pri)
include(../libs/verticallabel.pri)
include(../libs/waitingspinner.pri)



win32-msvc* {
    copy_grpc_files.commands = $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/*.dll)" "$$shell_path($${OUT_PWD}/build)"
    copy_quazip_files.commands = $(COPY_FILE) "$$shell_path($${QUAZIP_PATH}/bin/quazip1-qt6$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)"

    first.depends = $(first) copy_grpc_files copy_quazip_files

    export(first.depends)
    export(copy_grpc_files.commands)
    export(copy_quazip_files.commands)

    QMAKE_EXTRA_TARGETS += first copy_grpc_files copy_quazip_files
}
