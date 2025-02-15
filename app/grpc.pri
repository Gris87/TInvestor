VCPKG_PATH = E:/Qt/vcpkg

INCLUDEPATH += \
    $$PWD/messages/generated \
    $${VCPKG_PATH}/installed/x64-windows/include

DEFINES += \
    ABSL_CONSUME_DLL \
    PROTOBUF_USE_DLLS

LIBS += \
    -L$${VCPKG_PATH}/installed/x64-windows/debug/lib \
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
    -llibprotobufd \
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
    -lzlibd



copyfiles.commands = $(COPY_FILE) "$$shell_path($${VCPKG_PATH}/installed/x64-windows/debug/bin/*.dll)" "$$shell_path($${OUT_PWD}/build)"
first.depends = $(first) copyfiles

export(first.depends)
export(copyfiles.commands)

QMAKE_EXTRA_TARGETS += first copyfiles
