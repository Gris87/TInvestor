QT += core gui widgets charts network sql

CONFIG += c++20

TARGET = TInvestor
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



# TODO: Uncomment
#CONFIG (debug, debug|release) {
#    DEFINES += USE_SANDBOX
#}



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror -Wno-comment
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
    copy_dist_files.commands += $(COPY_FILE) "$$shell_path($${PWD}/dist/windows/tbank.ru.crt)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_dist_files.commands += (if exist "$$shell_path($${OUT_PWD}/build/tools)" rmdir /S /Q "$$shell_path($${OUT_PWD}/build/tools)") &&
}

linux* {
    copy_dist_files.commands =  $(COPY_FILE) "$$shell_path($${PWD}/dist/linux/icon.png)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_dist_files.commands =  $(COPY_FILE) "$$shell_path($${PWD}/dist/linux/notifier_icon.png)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_dist_files.commands += $(COPY_FILE) "$$shell_path($${PWD}/dist/linux/post_install.sh)" "$$shell_path($${OUT_PWD}/build)" &&
    copy_dist_files.commands += rm -rf "$$shell_path($${OUT_PWD}/build/tools)" &&
}

copy_dist_files.commands += $(MKDIR) "$$shell_path($${OUT_PWD}/build/tools)" &&
copy_dist_files.commands += $(MKDIR) "$$shell_path($${OUT_PWD}/build/tools/py3)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/generate-bidir-info)" "$$shell_path($${OUT_PWD}/build/tools/py3/generate-bidir-info)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/tinkoff-asap-trading)" "$$shell_path($${OUT_PWD}/build/tools/py3/tinkoff-asap-trading)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/tinkoff-bidir-trading)" "$$shell_path($${OUT_PWD}/build/tools/py3/tinkoff-bidir-trading)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/tinkoff-follower)" "$$shell_path($${OUT_PWD}/build/tools/py3/tinkoff-follower)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/tinvestor-notifier)" "$$shell_path($${OUT_PWD}/build/tools/py3/tinvestor-notifier)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/tinvestor-notifier-server)" "$$shell_path($${OUT_PWD}/build/tools/py3/tinvestor-notifier-server)" &&
copy_dist_files.commands += $(COPY_DIR) "$$shell_path($${PWD}/../tools/py3/tinvestor-telegram-bot)" "$$shell_path($${OUT_PWD}/build/tools/py3/tinvestor-telegram-bot)"



PROJECT_ROOT_PATH = ..

include($${PROJECT_ROOT_PATH}/app/app.pri)
include($${PROJECT_ROOT_PATH}/libs/libs.pri)



SOURCES += \
    src/main.cpp

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
