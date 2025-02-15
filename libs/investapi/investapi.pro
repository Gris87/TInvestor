CONFIG -= qt
CONFIG += c++20
CONFIG += staticlib

TARGET = investapi
TEMPLATE = lib




# Disable warnings - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS_WARN_ON -= -W3 -w34100 -w34189 -w44456 -w44457 -w44458 -wd4577 -wd4467
    QMAKE_CXXFLAGS_WARN_ON += /w
}
# Disable warnings - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



VCPKG_PATH = E:/Qt/vcpkg

INCLUDEPATH += \
    $${PWD}/messages/generated \
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



include(../../libs/investapi/investapi.pri)
