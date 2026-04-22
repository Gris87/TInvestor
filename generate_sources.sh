#!/bin/bash



PATH_TO_INVEST_API_PRI="libs/investapi/investapi.pri"
PATH_TO_APP_PRI="app/app.pri"
PATH_TO_TESTS_PRI="tests/tests.pri"
PATH_TO_NOTIFIER_PRI="tools/qt/notifier/notifier.pri"



########################################################################################################################

echo "# Generated with generate_sources.sh" > ${PATH_TO_INVEST_API_PRI}
echo "# find libs/investapi/ -name \\*.cc | sort | sed \"s/libs\\/investapi\\///g\" | sed \"s/\\.cc/\\.cc \\\\\\/g\"" >> ${PATH_TO_INVEST_API_PRI}

echo "SOURCES += \\" >> ${PATH_TO_INVEST_API_PRI}
find libs/investapi/ -name \*.cc | sort | sed "s/libs\/investapi\//    /g" | sed "s/\.cc/\.cc \\\/g" >> ${PATH_TO_INVEST_API_PRI}

echo "" >> ${PATH_TO_INVEST_API_PRI}
echo "# find libs/investapi/ -name \\*.h | sort | sed \"s/libs\\/investapi\\///g\" | sed \"s/\\.h/\\.h \\\\\\/g\"" >> ${PATH_TO_INVEST_API_PRI}

echo "HEADERS += \\" >> ${PATH_TO_INVEST_API_PRI}
find libs/investapi/ -name \*.h | sort | sed "s/libs\/investapi\//    /g" | sed "s/\.h/\.h \\\/g" >> ${PATH_TO_INVEST_API_PRI}

########################################################################################################################

echo "# Generated with generate_sources.sh" >  ${PATH_TO_APP_PRI}
echo "VPATH += \$\${PWD}"                   >> ${PATH_TO_APP_PRI}
echo "INCLUDEPATH += \$\${PWD}"             >> ${PATH_TO_APP_PRI}
echo ""                                     >> ${PATH_TO_APP_PRI}
echo ""                                     >> ${PATH_TO_APP_PRI}
echo ""                                     >> ${PATH_TO_APP_PRI}

echo "# find app/ -name \\*.cpp | sort | grep -ve test_ -ve app/src/main.cpp | sed \"s/app\\///g\" | sed \"s/\\.cpp/\\.cpp \\\\\\/g\"" >> ${PATH_TO_APP_PRI}

echo "SOURCES += \\" >> ${PATH_TO_APP_PRI}
find app/ -name \*.cpp | sort | grep -ve test_ -ve app/src/main.cpp | sed "s/app\//    /g" | sed "s/\.cpp/\.cpp \\\/g" >> ${PATH_TO_APP_PRI}

echo "" >> ${PATH_TO_APP_PRI}
echo "# find app/ -name \\*.h | sort | grep -ve _mock.h | sed \"s/app\\///g\" | sed \"s/\\.h/\\.h \\\\\\/g\"" >> ${PATH_TO_APP_PRI}

echo "HEADERS += \\" >> ${PATH_TO_APP_PRI}
find app/ -name \*.h | sort | grep -ve _mock.h | sed "s/app\//    /g" | sed "s/\.h/\.h \\\/g" >> ${PATH_TO_APP_PRI}

echo "" >> ${PATH_TO_APP_PRI}
echo "# find app/ -name \\*.ui | sort | sed \"s/app\\///g\" | sed \"s/\\.ui/\\.ui \\\\\\/g\"" >> ${PATH_TO_APP_PRI}

echo "FORMS += \\" >> ${PATH_TO_APP_PRI}
find app/ -name \*.ui | sort | sed "s/app\//    /g" | sed "s/\.ui/\.ui \\\/g" >> ${PATH_TO_APP_PRI}

########################################################################################################################

echo "# Generated with generate_sources.sh" > ${PATH_TO_TESTS_PRI}
echo "# find app/ -name test_\\*.cpp | sort | sed \"s/app\\///g\" | sed \"s/\\.cpp/\\.cpp \\\\\\/g\"" >> ${PATH_TO_TESTS_PRI}

echo "SOURCES += \\" >> ${PATH_TO_TESTS_PRI}
find app/ -name test_\*.cpp | sort | sed "s/app\//    /g" | sed "s/\.cpp/\.cpp \\\/g" >> ${PATH_TO_TESTS_PRI}

echo "" >> ${PATH_TO_TESTS_PRI}
echo "# find app/ -name \\*_mock.h | sort | sed \"s/app\\///g\" | sed \"s/\\.h/\\.h \\\\\\/g\"" >> ${PATH_TO_TESTS_PRI}

echo "HEADERS += \\" >> ${PATH_TO_TESTS_PRI}
find app/ -name \*_mock.h | sort | sed "s/app\//    /g" | sed "s/\.h/\.h \\\/g" >> ${PATH_TO_TESTS_PRI}

########################################################################################################################

echo "# Generated with generate_sources.sh" >  ${PATH_TO_NOTIFIER_PRI}
echo "VPATH += \$\${PWD}"                   >> ${PATH_TO_NOTIFIER_PRI}
echo "INCLUDEPATH += \$\${PWD}"             >> ${PATH_TO_NOTIFIER_PRI}
echo ""                                     >> ${PATH_TO_NOTIFIER_PRI}
echo ""                                     >> ${PATH_TO_NOTIFIER_PRI}
echo ""                                     >> ${PATH_TO_NOTIFIER_PRI}

echo "# find tools/qt/notifier/ -name \\*.cpp | sort | grep -ve test_ -ve tools/qt/notifier/src/main.cpp | sed \"s/tools\\/qt\\/notifier\\///g\" | sed \"s/\\.cpp/\\.cpp \\\\\\/g\"" >> ${PATH_TO_NOTIFIER_PRI}

echo "SOURCES += \\" >> ${PATH_TO_NOTIFIER_PRI}
find tools/qt/notifier/ -name \*.cpp | sort | grep -ve test_ -ve tools/qt/notifier/src/main.cpp | sed "s/tools\/qt\/notifier\//    /g" | sed "s/\.cpp/\.cpp \\\/g" >> ${PATH_TO_NOTIFIER_PRI}

echo "" >> ${PATH_TO_NOTIFIER_PRI}
echo "# find tools/qt/notifier/ -name \\*.h | sort | grep -ve _mock.h | sed \"s/tools\\/qt\\/notifier\\///g\" | sed \"s/\\.h/\\.h \\\\\\/g\"" >> ${PATH_TO_NOTIFIER_PRI}

echo "HEADERS += \\" >> ${PATH_TO_NOTIFIER_PRI}
find tools/qt/notifier/ -name \*.h | sort | grep -ve _mock.h | sed "s/tools\/qt\/notifier\//    /g" | sed "s/\.h/\.h \\\/g" >> ${PATH_TO_NOTIFIER_PRI}

echo "" >> ${PATH_TO_NOTIFIER_PRI}
echo "# find tools/qt/notifier/ -name \\*.ui | sort | sed \"s/tools\\/qt\\/notifier\\///g\" | sed \"s/\\.ui/\\.ui \\\\\\/g\"" >> ${PATH_TO_NOTIFIER_PRI}

echo "FORMS += \\" >> ${PATH_TO_NOTIFIER_PRI}
find tools/qt/notifier/ -name \*.ui | sort | sed "s/tools\/qt\/notifier\//    /g" | sed "s/\.ui/\.ui \\\/g" >> ${PATH_TO_NOTIFIER_PRI}
