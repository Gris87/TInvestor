CONFIG -= qt
CONFIG += c++20
CONFIG += staticlib

TARGET = simdjson
TEMPLATE = lib



# Disable warnings - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS_WARN_ON -= -W3 -w34100 -w34189 -w44456 -w44457 -w44458 -wd4577 -wd4467
    QMAKE_CXXFLAGS_WARN_ON += /w
} else {
    QMAKE_CXXFLAGS_WARN_ON += -w
}
# Disable warnings - END

# Extend number of sections - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += -bigobj
}
# Extend number of sections - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



SOURCES += \
    src/simdjson/simdjson.cpp

HEADERS += \
    src/simdjson/simdjson.h \
    src/simdjson/simdjson_wrapped.h
