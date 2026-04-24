# Generated with generate_sources.sh
VPATH += $${PWD}
INCLUDEPATH += $${PWD}



# find tools/qt/notifier/ -name \*.cpp | sort | grep -ve test_ -ve tools/qt/notifier/src/main.cpp | sed "s/tools\/qt\/notifier\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/config/config.cpp \
    src/main/mainwindow.cpp \
    src/utils/logger/logger.cpp \
    src/utils/settingseditor/settingseditor.cpp \
    src/utils/style/darkpalette.cpp \
    src/widgets/trayicon/trayicon.cpp \
    src/widgets/trayicon/trayiconfactory.cpp \

# find tools/qt/notifier/ -name \*.h | sort | grep -ve _mock.h | sed "s/tools\/qt\/notifier\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/config.h \
    src/config/iconfig.h \
    src/main/mainwindow.h \
    src/utils/logger/logger.h \
    src/utils/settingseditor/isettingseditor.h \
    src/utils/settingseditor/settingseditor.h \
    src/utils/style/darkpalette.h \
    src/widgets/trayicon/itrayicon.h \
    src/widgets/trayicon/itrayiconfactory.h \
    src/widgets/trayicon/trayicon.h \
    src/widgets/trayicon/trayiconfactory.h \

# find tools/qt/notifier/ -name \*.ui | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.ui/\.ui \\\/g"
FORMS += \
    src/main/mainwindow.ui \
