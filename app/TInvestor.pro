QT += core gui widgets charts network sql

CONFIG += c++20

TARGET = TInvestor
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



CONFIG (debug, debug|release) {
    DEFINES += USE_SANDBOX
}



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror
}
# Warnings as errors - END

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



win32* {
    copy_dist_files.commands =  $(COPY_FILE) "$$shell_path($${PWD}/dist/windows/post_install.bat)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_dist_files.commands += $(COPY_FILE) "$$shell_path($${PWD}/dist/windows/tbank.ru.crt)" "$$shell_path($${OUT_PWD}/build)"
}

linux* {
    copy_dist_files.commands =  $(COPY_FILE) "$$shell_path($${PWD}/dist/linux/icon.png)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_dist_files.commands += $(COPY_FILE) "$$shell_path($${PWD}/dist/linux/post_install.sh)" "$$shell_path($${OUT_PWD}/build)"
}



include(../app/TInvestor.pri)
include(../libs/libs.pri)



SOURCES += \
    src/main.cpp

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
