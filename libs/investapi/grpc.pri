VCPKG_PATH = E:/Qt/vcpkg

INCLUDEPATH += \
    $${VCPKG_PATH}/installed/x64-windows/include

CONFIG (debug, debug|release) {
    VCPKG_LIBS = $${VCPKG_PATH}/installed/x64-windows/debug/lib
    VCPKG_DLLS = $${VCPKG_PATH}/installed/x64-windows/debug/bin
    DEBUG_SUFFIX = d
} else {
    VCPKG_LIBS = $${VCPKG_PATH}/installed/x64-windows/lib
    VCPKG_DLLS = $${VCPKG_PATH}/installed/x64-windows/bin
    DEBUG_SUFFIX =
}

LIBS += \
    -L$${VCPKG_LIBS} \
    -labseil_dll \
    -labsl_flags_commandlineflag \
    -labsl_flags_commandlineflag_internal \
    -labsl_flags_config \
    -labsl_flags_internal \
    -labsl_flags_marshalling \
    -labsl_flags_private_handle_accessor \
    -labsl_flags_program_name \
    -labsl_flags_reflection \
    -laddress_sorting \
    -lcares \
    -lgpr \
    -lgrpc \
    -lgrpc++ \
    -lgrpc++_reflection \
    -llibcrypto \
    -llibprotobuf$${DEBUG_SUFFIX} \
    -llibssl \
    -lre2 \
    -lupb_base_lib \
    -lupb_json_lib \
    -lupb_mem_lib \
    -lupb_message_lib \
    -lupb_mini_descriptor_lib \
    -lupb_textformat_lib \
    -lupb_wire_lib \
    -lutf8_range \
    -lzlib$${DEBUG_SUFFIX}



copyfiles.commands = $(COPY_FILE) "$$shell_path($${VCPKG_DLLS}/*.dll)" "$$shell_path($${OUT_PWD}/build)"
first.depends = $(first) copyfiles

export(first.depends)
export(copyfiles.commands)

QMAKE_EXTRA_TARGETS += first copyfiles
