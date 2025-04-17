include(../libs/investapi.pri)
include(../libs/quazip.pri)
include(../libs/simplecrypt.pri)
include(../libs/verticallabel.pri)
include(../libs/waitingspinner.pri)



win32-msvc* {
    copy_grpc_files.commands = $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/*.dll)" "$$shell_path($${OUT_PWD}/build)"
    copy_quazip_files.commands = $(COPY_FILE) "$$shell_path($${QUAZIP_PATH}/install/bin/quazip1-qt6$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)"

    first.depends = $(first) copy_grpc_files copy_quazip_files

    export(first.depends)
    export(copy_grpc_files.commands)
    export(copy_quazip_files.commands)

    QMAKE_EXTRA_TARGETS += first copy_grpc_files copy_quazip_files
} else {
    copy_qt_files.commands = $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Core.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Gui.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Widgets.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Network.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Sql.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6DBus.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Core5Compat.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libicui18n.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libicuuc.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libicudata.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/plugins/platforms/libqwayland-generic.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/plugins/platforms/libqwayland-egl.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/plugins/platforms/libqxcb.so*)" "$$shell_path($${OUT_PWD}/build)"
    copy_quazip_files.commands = $(COPY_FILE) --no-dereference "$$shell_path($${QUAZIP_PATH}/install/lib/libquazip1-qt6$${DEBUG_SUFFIX}.so*)" "$$shell_path($${OUT_PWD}/build)"

    first.depends = $(first) copy_qt_files copy_quazip_files

    export(first.depends)
    export(copy_qt_files.commands)
    export(copy_quazip_files.commands)

    QMAKE_EXTRA_TARGETS += first copy_qt_files copy_quazip_files
}
