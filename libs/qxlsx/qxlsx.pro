QT += core gui-private

CONFIG += c++20
CONFIG += staticlib

TARGET = qxslx
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
    src/qxlsx/xlsxabstractooxmlfile.cpp \
    src/qxlsx/xlsxabstractsheet.cpp \
    src/qxlsx/xlsxcell.cpp \
    src/qxlsx/xlsxcellformula.cpp \
    src/qxlsx/xlsxcelllocation.cpp \
    src/qxlsx/xlsxcellrange.cpp \
    src/qxlsx/xlsxcellreference.cpp \
    src/qxlsx/xlsxchart.cpp \
    src/qxlsx/xlsxchartsheet.cpp \
    src/qxlsx/xlsxcolor.cpp \
    src/qxlsx/xlsxconditionalformatting.cpp \
    src/qxlsx/xlsxcontenttypes.cpp \
    src/qxlsx/xlsxdatavalidation.cpp \
    src/qxlsx/xlsxdatetype.cpp \
    src/qxlsx/xlsxdocpropsapp.cpp \
    src/qxlsx/xlsxdocpropscore.cpp \
    src/qxlsx/xlsxdocument.cpp \
    src/qxlsx/xlsxdrawing.cpp \
    src/qxlsx/xlsxdrawinganchor.cpp \
    src/qxlsx/xlsxformat.cpp \
    src/qxlsx/xlsxmediafile.cpp \
    src/qxlsx/xlsxnumformatparser.cpp \
    src/qxlsx/xlsxrelationships.cpp \
    src/qxlsx/xlsxrichstring.cpp \
    src/qxlsx/xlsxsharedstrings.cpp \
    src/qxlsx/xlsxsimpleooxmlfile.cpp \
    src/qxlsx/xlsxstyles.cpp \
    src/qxlsx/xlsxtheme.cpp \
    src/qxlsx/xlsxutility.cpp \
    src/qxlsx/xlsxworkbook.cpp \
    src/qxlsx/xlsxworksheet.cpp \
    src/qxlsx/xlsxzipreader.cpp \
    src/qxlsx/xlsxzipwriter.cpp

HEADERS += \
    src/qxlsx/xlsxabstractooxmlfile.h \
    src/qxlsx/xlsxabstractooxmlfile_p.h \
    src/qxlsx/xlsxabstractsheet.h \
    src/qxlsx/xlsxabstractsheet_p.h \
    src/qxlsx/xlsxcell.h \
    src/qxlsx/xlsxcell_p.h \
    src/qxlsx/xlsxcellformula.h \
    src/qxlsx/xlsxcellformula_p.h \
    src/qxlsx/xlsxcelllocation.h \
    src/qxlsx/xlsxcellrange.h \
    src/qxlsx/xlsxcellreference.h \
    src/qxlsx/xlsxchart.h \
    src/qxlsx/xlsxchart_p.h \
    src/qxlsx/xlsxchartsheet.h \
    src/qxlsx/xlsxchartsheet_p.h \
    src/qxlsx/xlsxcolor_p.h \
    src/qxlsx/xlsxconditionalformatting.h \
    src/qxlsx/xlsxconditionalformatting_p.h \
    src/qxlsx/xlsxcontenttypes_p.h \
    src/qxlsx/xlsxdatavalidation.h \
    src/qxlsx/xlsxdatavalidation_p.h \
    src/qxlsx/xlsxdatetype.h \
    src/qxlsx/xlsxdocpropsapp_p.h \
    src/qxlsx/xlsxdocpropscore_p.h \
    src/qxlsx/xlsxdocument.h \
    src/qxlsx/xlsxdocument_p.h \
    src/qxlsx/xlsxdrawing_p.h \
    src/qxlsx/xlsxdrawinganchor_p.h \
    src/qxlsx/xlsxformat.h \
    src/qxlsx/xlsxformat_p.h \
    src/qxlsx/xlsxglobal.h \
    src/qxlsx/xlsxmediafile_p.h \
    src/qxlsx/xlsxnumformatparser_p.h \
    src/qxlsx/xlsxrelationships_p.h \
    src/qxlsx/xlsxrichstring.h \
    src/qxlsx/xlsxrichstring_p.h \
    src/qxlsx/xlsxsharedstrings_p.h \
    src/qxlsx/xlsxsimpleooxmlfile_p.h \
    src/qxlsx/xlsxstyles_p.h \
    src/qxlsx/xlsxtheme_p.h \
    src/qxlsx/xlsxutility_p.h \
    src/qxlsx/xlsxworkbook.h \
    src/qxlsx/xlsxworkbook_p.h \
    src/qxlsx/xlsxworksheet.h \
    src/qxlsx/xlsxworksheet_p.h \
    src/qxlsx/xlsxzipreader_p.h \
    src/qxlsx/xlsxzipwriter_p.h
