isEmpty(GOOGLETEST_PATH):GOOGLETEST_PATH=$$(GOOGLETEST_PATH)

isEmpty(GOOGLETEST_PATH) {
    win32* {
        GOOGLETEST_PATH = "E:/Qt/googletest-1.16.0"
    } else {
        GOOGLETEST_PATH = "/home/gris/Qt/googletest-1.16.0"
    }

    !isEmpty(GOOGLETEST_PATH) {
        warning("Using googletest src dir specified at Qt Creator wizard")
        message("set GOOGLETEST_PATH as environment variable or qmake variable to get rid of this message")
    }
}

!isEmpty(GOOGLETEST_PATH): {
    GTEST_SRCDIR = "$${GOOGLETEST_PATH}/googletest"
    GMOCK_SRCDIR = "$${GOOGLETEST_PATH}/googlemock"
} else: unix {
    exists(/usr/src/gtest):GTEST_SRCDIR=/usr/src/gtest
    exists(/usr/src/gmock):GMOCK_SRCDIR=/usr/src/gmock
    !isEmpty(GTEST_SRCDIR): message("Using gtest from system")
}

requires(exists($${GTEST_SRCDIR}):exists($${GMOCK_SRCDIR}))

!isEmpty(GTEST_SRCDIR) {
    INCLUDEPATH *= \
        $${GTEST_SRCDIR} \
        $${GTEST_SRCDIR}/include

    SOURCES += \
        $${GTEST_SRCDIR}/src/gtest-all.cc
}

!isEmpty(GMOCK_SRCDIR) {
    INCLUDEPATH *= \
        $${GMOCK_SRCDIR} \
        $${GMOCK_SRCDIR}/include

    SOURCES += \
        $${GMOCK_SRCDIR}/src/gmock-all.cc
}
