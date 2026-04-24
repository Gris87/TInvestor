# Generated with generate_sources.sh
# find tools/qt/notifier/ -name test_\*.cpp | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/config/test_config.cpp \
    src/main/test_mainwindow.cpp \
    src/utils/logger/test_logger.cpp \
    src/utils/settingseditor/test_settingseditor.cpp \
    src/utils/style/test_darkpalette.cpp \
    src/widgets/trayicon/test_trayicon.cpp \
    src/widgets/trayicon/test_trayiconfactory.cpp \

# find tools/qt/notifier/ -name \*_mock.h | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/iconfig_mock.h \
    src/utils/settingseditor/isettingseditor_mock.h \
    src/widgets/trayicon/itrayicon_mock.h \
    src/widgets/trayicon/itrayiconfactory_mock.h \
