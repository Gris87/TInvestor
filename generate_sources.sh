#!/bin/bash



PATH_TO_TINVESTOR_PRI="app/TInvestor.pri"
PATH_TO_TESTS_PRI="test/tests.pri"



echo "VPATH = \$\$PWD"        >  ${PATH_TO_TINVESTOR_PRI}
echo "INCLUDEPATH += \$\$PWD" >> ${PATH_TO_TINVESTOR_PRI}
echo ""                       >> ${PATH_TO_TINVESTOR_PRI}
echo ""                       >> ${PATH_TO_TINVESTOR_PRI}
echo ""                       >> ${PATH_TO_TINVESTOR_PRI}

echo "# find app/ -name \\*.cpp | grep -ve test_ -ve app/src/main.cpp | sed \"s/app\\///g\" | sed \"s/\\.cpp/\\.cpp \\\\\\/g\"" >> ${PATH_TO_TINVESTOR_PRI}

echo "SOURCES += \\" >> ${PATH_TO_TINVESTOR_PRI}
find app/ -name \*.cpp | grep -ve test_ -ve app/src/main.cpp | sed "s/app\//    /g" | sed "s/\.cpp/\.cpp \\\/g" >> ${PATH_TO_TINVESTOR_PRI}

echo "" >> ${PATH_TO_TINVESTOR_PRI}
echo "# find app/ -name \\*.h | grep -ve _mock.h | sed \"s/app\\///g\" | sed \"s/\\.h/\\.h \\\\\\/g\"" >> ${PATH_TO_TINVESTOR_PRI}

echo "HEADERS += \\" >> ${PATH_TO_TINVESTOR_PRI}
find app/ -name \*.h | grep -ve _mock.h | sed "s/app\//    /g" | sed "s/\.h/\.h \\\/g" >> ${PATH_TO_TINVESTOR_PRI}

echo "" >> ${PATH_TO_TINVESTOR_PRI}
echo "# find app/ -name \\*.ui | sed \"s/app\\///g\" | sed \"s/\\.ui/\\.ui \\\\\\/g\"" >> ${PATH_TO_TINVESTOR_PRI}

echo "FORMS += \\" >> ${PATH_TO_TINVESTOR_PRI}
find app/ -name \*.ui | sed "s/app\//    /g" | sed "s/\.ui/\.ui \\\/g" >> ${PATH_TO_TINVESTOR_PRI}



echo "# find app/ -name test_\\*.cpp | sed \"s/app\\///g\" | sed \"s/\\.cpp/\\.cpp \\\\\\/g\"" > ${PATH_TO_TESTS_PRI}

echo "SOURCES += \\" >> ${PATH_TO_TESTS_PRI}
find app/ -name test_\*.cpp | sed "s/app\//    /g" | sed "s/\.cpp/\.cpp \\\/g" >> ${PATH_TO_TESTS_PRI}

echo "" >> ${PATH_TO_TESTS_PRI}
echo "# find app/ -name \\*_mock.h | sed \"s/app\\///g\" | sed \"s/\\.h/\\.h \\\\\\/g\"" >> ${PATH_TO_TESTS_PRI}

echo "HEADERS += \\" >> ${PATH_TO_TESTS_PRI}
find app/ -name \*_mock.h | sed "s/app\//    /g" | sed "s/\.h/\.h \\\/g" >> ${PATH_TO_TESTS_PRI}
