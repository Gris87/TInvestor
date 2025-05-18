include(../libs/investapi.pri)
include(../libs/quazip.pri)
include(../libs/qxslx.pri)
include(../libs/simplecrypt.pri)
include(../libs/verticallabel.pri)
include(../libs/waitingspinner.pri)
include(../libs/zlib.pri)



win32* {
    copy_qt_files.commands =  $(COPY_FILE) "$$shell_path($$(QTDIR)/bin/Qt6Core$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/bin/Qt6Gui$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/bin/Qt6Widgets$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/bin/Qt6Network$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/bin/Qt6Sql$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/bin/Qt6Core5Compat$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(CHK_DIR_EXISTS) "$$shell_path($${OUT_PWD}/build/platforms)" $(MKDIR) "$$shell_path($${OUT_PWD}/build/platforms)" &&
    copy_qt_files.commands += $(CHK_DIR_EXISTS) "$$shell_path($${OUT_PWD}/build/sqldrivers)" $(MKDIR) "$$shell_path($${OUT_PWD}/build/sqldrivers)" &&
    copy_qt_files.commands += $(CHK_DIR_EXISTS) "$$shell_path($${OUT_PWD}/build/tls)" $(MKDIR) "$$shell_path($${OUT_PWD}/build/tls)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/plugins/platforms/qwindows$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build/platforms)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/plugins/sqldrivers/qsqlite$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build/sqldrivers)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/plugins/tls/qopensslbackend$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build/tls)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(WINDIR)/System32/ucrtbase$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(WINDIR)/System32/msvcp140$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(WINDIR)/System32/msvcp140_1$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(WINDIR)/System32/msvcp140_2$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(WINDIR)/System32/vcruntime140$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(WINDIR)/System32/vcruntime140_1$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)"

    copy_grpc_files.commands =  $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/abseil_dll.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_grpc_files.commands += $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/cares.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_grpc_files.commands += $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/libcrypto-3-x64.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_grpc_files.commands += $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/libprotobuf$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_grpc_files.commands += $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/libssl-3-x64.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_grpc_files.commands += $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/re2.dll)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_grpc_files.commands += $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/zlib$${DEBUG_SUFFIX}1.dll)" "$$shell_path($${OUT_PWD}/build)"

    copy_zlib_files.commands = $(COPY_FILE) "$$shell_path($${ZLIB_PATH}/install/bin/zlib$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)"

    copy_quazip_files.commands = $(COPY_FILE) "$$shell_path($${QUAZIP_PATH}/install/bin/quazip1-qt6$${DEBUG_SUFFIX}.dll)" "$$shell_path($${OUT_PWD}/build)"

    first.depends = $(first) copy_qt_files copy_grpc_files copy_zlib_files copy_quazip_files copy_dist_files

    export(first.depends)
    export(copy_qt_files.commands)
    export(copy_grpc_files.commands)
    export(copy_zlib_files.commands)
    export(copy_quazip_files.commands)
    export(copy_dist_files.commands)

    QMAKE_EXTRA_TARGETS += first copy_qt_files copy_grpc_files copy_zlib_files copy_quazip_files copy_dist_files
} else {
    copy_qt_files.commands =  $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Core.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Gui.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Widgets.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Network.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Sql.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6Core5Compat.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6DBus.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libQt6XcbQpa.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libicui18n.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libicuuc.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path($$(QTDIR)/lib/libicudata.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(MKDIR) "$$shell_path($${OUT_PWD}/build/platforms)" &&
    copy_qt_files.commands += $(MKDIR) "$$shell_path($${OUT_PWD}/build/sqldrivers)" &&
    copy_qt_files.commands += $(MKDIR) "$$shell_path($${OUT_PWD}/build/tls)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/plugins/platforms/libqxcb.so)" "$$shell_path($${OUT_PWD}/build/platforms)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/plugins/sqldrivers/libqsqlite.so)" "$$shell_path($${OUT_PWD}/build/sqldrivers)" &&
    copy_qt_files.commands += $(COPY_FILE) "$$shell_path($$(QTDIR)/plugins/tls/libqopensslbackend.so)" "$$shell_path($${OUT_PWD}/build/tls)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libEGL.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxkbcommon.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxkbcommon-x11.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-cursor.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-icccm.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-image.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-keysyms.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-randr.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-render.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-render-util.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-shape.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-shm.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-sync.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-util.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-xfixes.so*)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_qt_files.commands += $(COPY_FILE) --no-dereference "$$shell_path(/lib/x86_64-linux-gnu/libxcb-xkb.so*)" "$$shell_path($${OUT_PWD}/build)"

    copy_quazip_files.commands = $(COPY_FILE) --no-dereference "$$shell_path($${QUAZIP_PATH}/install/lib/libquazip1-qt6$${DEBUG_SUFFIX}.so*)" "$$shell_path($${OUT_PWD}/build)"

    first.depends = $(first) copy_qt_files copy_quazip_files copy_dist_files

    export(first.depends)
    export(copy_qt_files.commands)
    export(copy_quazip_files.commands)
    export(copy_dist_files.commands)

    QMAKE_EXTRA_TARGETS += first copy_qt_files copy_quazip_files copy_dist_files
}
