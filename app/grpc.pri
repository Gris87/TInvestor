VPATH += $$PWD/messages/generated

INCLUDEPATH += $$PWD/messages/generated
INCLUDEPATH += E:/Qt/vcpkg/installed/x64-windows/include

DEFINES += ABSL_CONSUME_DLL
DEFINES += PROTOBUF_USE_DLLS

LIBS += -LE:/Qt/vcpkg/installed/x64-windows/debug/lib
LIBS += -labseil_dll
LIBS += -labsl_flags_commandlineflag
LIBS += -labsl_flags_commandlineflag_internal
LIBS += -labsl_flags_config
LIBS += -labsl_flags_internal
LIBS += -labsl_flags_marshalling
LIBS += -labsl_flags_private_handle_accessor
LIBS += -labsl_flags_program_name
LIBS += -labsl_flags_reflection
LIBS += -laddress_sorting
LIBS += -lcares
LIBS += -lgpr
LIBS += -lgrpc
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
LIBS += -llibcrypto
LIBS += -llibprotobufd
LIBS += -llibssl
LIBS += -lre2
LIBS += -lupb_base_lib
LIBS += -lupb_json_lib
LIBS += -lupb_mem_lib
LIBS += -lupb_message_lib
LIBS += -lupb_mini_descriptor_lib
LIBS += -lupb_textformat_lib
LIBS += -lupb_wire_lib
LIBS += -lutf8_range
LIBS += -lzlibd
