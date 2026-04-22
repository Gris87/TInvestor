# Generated with generate_sources.sh
VPATH += $${PWD}
INCLUDEPATH += $${PWD}



# find app/ -name \*.cpp | sort | grep -ve test_ -ve app/src/main.cpp | sed "s/app\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/main/mainwindow.cpp \

# find app/ -name \*.h | sort | grep -ve _mock.h | sed "s/app\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/main/mainwindow.h \

# find app/ -name \*.ui | sort | sed "s/app\///g" | sed "s/\.ui/\.ui \\\/g"
FORMS += \
    src/main/mainwindow.ui \
