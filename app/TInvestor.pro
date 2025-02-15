QT += core gui widgets sql websockets

CONFIG += c++20

TARGET = TInvestor
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



# Release optimization - BEGIN
QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
# Release optimization - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$$TARGET/objs
MOC_DIR     = build/gen/$$TARGET/moc
RCC_DIR     = build/gen/$$TARGET/rcc
UI_DIR      = build/gen/$$TARGET/ui
# Target dirs - END



include(../app/grpc.pri)
include(../app/TInvestor.pri)



SOURCES += \
    src/main.cpp

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
