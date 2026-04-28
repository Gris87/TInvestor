# Generated with generate_sources.sh
# find tools/qt/notifier/ -name test_\*.cpp | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/config/test_config.cpp \
    src/dialogs/settingsdialog/test_settingsdialog.cpp \
    src/dialogs/settingsdialog/test_settingsdialogfactory.cpp \
    src/main/test_mainwindow.cpp \
    src/threads/request/test_requestthread.cpp \
    src/utils/autorunenabler/test_autorunenabler.cpp \
    src/utils/fs/dir/test_dir.cpp \
    src/utils/fs/dir/test_dirfactory.cpp \
    src/utils/fs/file/test_file.cpp \
    src/utils/fs/file/test_filefactory.cpp \
    src/utils/logger/test_logger.cpp \
    src/utils/settingseditor/test_settingseditor.cpp \
    src/utils/style/test_darkpalette.cpp \
    src/widgets/trayicon/test_trayicon.cpp \
    src/widgets/trayicon/test_trayiconfactory.cpp \

# find tools/qt/notifier/ -name \*_mock.h | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/iconfig_mock.h \
    src/dialogs/settingsdialog/isettingsdialog_mock.h \
    src/dialogs/settingsdialog/isettingsdialogfactory_mock.h \
    src/threads/request/irequestthread_mock.h \
    src/utils/autorunenabler/iautorunenabler_mock.h \
    src/utils/fs/dir/idir_mock.h \
    src/utils/fs/dir/idirfactory_mock.h \
    src/utils/fs/file/ifile_mock.h \
    src/utils/fs/file/ifilefactory_mock.h \
    src/utils/settingseditor/isettingseditor_mock.h \
    src/widgets/trayicon/itrayicon_mock.h \
    src/widgets/trayicon/itrayiconfactory_mock.h \
