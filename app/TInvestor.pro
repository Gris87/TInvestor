QT += core gui widgets sql websockets

CONFIG += c++20

TARGET = TInvestor
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



include(../app/investapi.pri)
include(../app/TInvestor.pri)



SOURCES += \
    src/main.cpp

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
