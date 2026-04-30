# Generated with generate_sources.sh
VPATH += $${PWD}
INCLUDEPATH += $${PWD}



# find tools/qt/notifier/ -name \*.cpp | sort | grep -ve test_ -ve tools/qt/notifier/src/main.cpp | sed "s/tools\/qt\/notifier\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/config/config.cpp \
    src/db/notifications/notificationsdatabase.cpp \
    src/dialogs/settingsdialog/settingsdialog.cpp \
    src/dialogs/settingsdialog/settingsdialogfactory.cpp \
    src/domain/filter/filter.cpp \
    src/domain/notification/notificationinfo.cpp \
    src/main/mainwindow.cpp \
    src/threads/parallelhelper/parallelhelperthread.cpp \
    src/threads/request/requestthread.cpp \
    src/utils/autorunenabler/autorunenabler.cpp \
    src/utils/exception/exception.cpp \
    src/utils/fs/dir/dir.cpp \
    src/utils/fs/dir/dirfactory.cpp \
    src/utils/fs/file/file.cpp \
    src/utils/fs/file/filefactory.cpp \
    src/utils/http/httpclient.cpp \
    src/utils/logger/logger.cpp \
    src/utils/optimizer/optimizer.cpp \
    src/utils/settingseditor/settingseditor.cpp \
    src/utils/style/darkpalette.cpp \
    src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.cpp \
    src/widgets/tablemodels/notificationstablemodel/notificationstablemodelfactory.cpp \
    src/widgets/trayicon/trayicon.cpp \
    src/widgets/trayicon/trayiconfactory.cpp \

# find tools/qt/notifier/ -name \*.h | sort | grep -ve _mock.h | sed "s/tools\/qt\/notifier\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/config.h \
    src/config/iconfig.h \
    src/db/notifications/inotificationsdatabase.h \
    src/db/notifications/notificationsdatabase.h \
    src/dialogs/settingsdialog/isettingsdialog.h \
    src/dialogs/settingsdialog/isettingsdialogfactory.h \
    src/dialogs/settingsdialog/settingsdialog.h \
    src/dialogs/settingsdialog/settingsdialogfactory.h \
    src/domain/filter/filter.h \
    src/domain/notification/notificationinfo.h \
    src/main/mainwindow.h \
    src/threads/parallelhelper/parallelhelperthread.h \
    src/threads/request/irequestthread.h \
    src/threads/request/requestthread.h \
    src/utils/autorunenabler/autorunenabler.h \
    src/utils/autorunenabler/iautorunenabler.h \
    src/utils/exception/exception.h \
    src/utils/fs/dir/dir.h \
    src/utils/fs/dir/dirfactory.h \
    src/utils/fs/dir/idir.h \
    src/utils/fs/dir/idirfactory.h \
    src/utils/fs/file/file.h \
    src/utils/fs/file/filefactory.h \
    src/utils/fs/file/ifile.h \
    src/utils/fs/file/ifilefactory.h \
    src/utils/http/httpclient.h \
    src/utils/http/ihttpclient.h \
    src/utils/logger/logger.h \
    src/utils/optimizer/ioptimizer.h \
    src/utils/optimizer/optimizer.h \
    src/utils/settingseditor/isettingseditor.h \
    src/utils/settingseditor/settingseditor.h \
    src/utils/style/darkpalette.h \
    src/widgets/tablemodels/notificationstablemodel/comparators.h \
    src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel.h \
    src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory.h \
    src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.h \
    src/widgets/tablemodels/notificationstablemodel/notificationstablemodelfactory.h \
    src/widgets/trayicon/itrayicon.h \
    src/widgets/trayicon/itrayiconfactory.h \
    src/widgets/trayicon/trayicon.h \
    src/widgets/trayicon/trayiconfactory.h \

# find tools/qt/notifier/ -name \*.ui | sort | sed "s/tools\/qt\/notifier\///g" | sed "s/\.ui/\.ui \\\/g"
FORMS += \
    src/dialogs/settingsdialog/settingsdialog.ui \
    src/main/mainwindow.ui \
