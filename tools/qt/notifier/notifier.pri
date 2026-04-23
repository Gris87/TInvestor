# Generated with generate_sources.sh
VPATH += $${PWD}
INCLUDEPATH += $${PWD}



# find tools/qt/notifier/ -name \*.cpp | sort | grep -ve test_ -ve tools/qt/notifier/src/main.cpp | sed "s/tools\/qt\/notifier\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/main/mainwindow.cpp \
    src/utils/logger/logger.cpp \
    src/utils/style/darkpalette.cpp \

# find tools/qt/notifier/ -name \*.h | sort | grep -ve _mock.h | sed "s/tools\/qt\/notifier\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/main/mainwindow.h \
    src/utils/logger/logger.h \
    src/utils/style/darkpalette.h \

# find tools/qt/notifier/ -name \*.ui | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.ui/\.ui \\\/g"
FORMS += \
    src/main/mainwindow.ui \
