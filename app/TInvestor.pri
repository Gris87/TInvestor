# Generated with generate_sources.sh
VPATH += $${PWD}
INCLUDEPATH += $${PWD}



# find app/ -name \*.cpp | sort | grep -ve test_ -ve app/src/main.cpp | sed "s/app\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/config/config.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1config.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2config.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3config.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.cpp \
    src/config/decisions/decisionmakerconfig.cpp \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.cpp \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1config.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2config.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3config.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidgetfactory.cpp \
    src/db/stocks/stocksdatabase.cpp \
    src/db/user/userdatabase.cpp \
    src/dialogs/authdialog/authdialog.cpp \
    src/dialogs/authdialog/authdialogfactory.cpp \
    src/dialogs/orderwavesdialog/orderwavesdialog.cpp \
    src/dialogs/orderwavesdialog/orderwavesdialogfactory.cpp \
    src/dialogs/settingsdialog/settingsdialog.cpp \
    src/dialogs/settingsdialog/settingsdialogfactory.cpp \
    src/dialogs/startautopilotdialog/startautopilotdialog.cpp \
    src/dialogs/startautopilotdialog/startautopilotdialogfactory.cpp \
    src/dialogs/startsimulationdialog/startsimulationdialog.cpp \
    src/dialogs/startsimulationdialog/startsimulationdialogfactory.cpp \
    src/domain/filter/filter.cpp \
    src/domain/orderbook/orderbook.cpp \
    src/domain/quotation/quotation.cpp \
    src/domain/stocks/stock.cpp \
    src/domain/stocks/stockmeta.cpp \
    src/domain/stocks/stockoperational.cpp \
    src/domain/user/account.cpp \
    src/domain/user/user.cpp \
    src/grpc/grpcclient.cpp \
    src/grpc/rawgrpcclient.cpp \
    src/grpc/utils.cpp \
    src/main/mainwindow.cpp \
    src/storage/stocks/stocksstorage.cpp \
    src/storage/user/userstorage.cpp \
    src/threads/cleanup/cleanupthread.cpp \
    src/threads/lastprice/lastpricethread.cpp \
    src/threads/makedecision/makedecisionthread.cpp \
    src/threads/orderbook/orderbookthread.cpp \
    src/threads/pricecollect/pricecollectthread.cpp \
    src/threads/userupdate/userupdatethread.cpp \
    src/utils/autorunenabler/autorunenabler.cpp \
    src/utils/fs/dir/dir.cpp \
    src/utils/fs/dir/dirfactory.cpp \
    src/utils/fs/file/file.cpp \
    src/utils/fs/file/filefactory.cpp \
    src/utils/fs/zip/qzip/qzip.cpp \
    src/utils/fs/zip/qzip/qzipfactory.cpp \
    src/utils/fs/zip/qzipfile/qzipfile.cpp \
    src/utils/fs/zip/qzipfile/qzipfilefactory.cpp \
    src/utils/http/httpclient.cpp \
    src/utils/logger/logger.cpp \
    src/utils/messagebox/messagebox.cpp \
    src/utils/processrunner/processrunner.cpp \
    src/utils/processrunner/processrunnerfactory.cpp \
    src/utils/settingseditor/settingseditor.cpp \
    src/utils/style/darkpalette.cpp \
    src/utils/timeutils/timeutils.cpp \
    src/widgets/accountchartwidget/accountchartwidget.cpp \
    src/widgets/accountchartwidget/accountchartwidgetfactory.cpp \
    src/widgets/decisionmakerwidget/decisionmakerwidget.cpp \
    src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.cpp \
    src/widgets/logstablewidget/logstablewidget.cpp \
    src/widgets/logstablewidget/logstablewidgetfactory.cpp \
    src/widgets/operationstablewidget/operationstablewidget.cpp \
    src/widgets/operationstablewidget/operationstablewidgetfactory.cpp \
    src/widgets/orderwaveswidget/orderwaveswidget.cpp \
    src/widgets/orderwaveswidget/orderwaveswidgetfactory.cpp \
    src/widgets/portfoliotablewidget/portfoliotablewidget.cpp \
    src/widgets/portfoliotablewidget/portfoliotablewidgetfactory.cpp \
    src/widgets/stockscontrolswidget/stockscontrolswidget.cpp \
    src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.cpp \
    src/widgets/stockstablewidget/stockstablewidget.cpp \
    src/widgets/stockstablewidget/stockstablewidgetfactory.cpp \
    src/widgets/tableitems/actions/actionstableitemwidget.cpp \
    src/widgets/tableitems/actions/actionstableitemwidgetfactory.cpp \
    src/widgets/tableitems/paybacktableitem.cpp \
    src/widgets/tableitems/pricechangetableitem.cpp \
    src/widgets/tableitems/pricetableitem.cpp \
    src/widgets/tableitems/stock/stocktableitemwidget.cpp \
    src/widgets/tableitems/stock/stocktableitemwidgetfactory.cpp \
    src/widgets/tableitems/turnovertableitem.cpp \
    src/widgets/tablerecords/stockstablerecord/stockstablerecord.cpp \
    src/widgets/tablerecords/stockstablerecord/stockstablerecordfactory.cpp \
    src/widgets/trayicon/trayicon.cpp \
    src/widgets/trayicon/trayiconfactory.cpp \

# find app/ -name \*.h | sort | grep -ve _mock.h | sed "s/app\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/config.h \
    src/config/decisions/buy/buydecision1config/buydecision1config.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h \
    src/config/decisions/buy/buydecision1config/ibuydecision1config.h \
    src/config/decisions/buy/buydecision2config/buydecision2config.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h \
    src/config/decisions/buy/buydecision2config/ibuydecision2config.h \
    src/config/decisions/buy/buydecision3config/buydecision3config.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h \
    src/config/decisions/buy/buydecision3config/ibuydecision3config.h \
    src/config/decisions/decisionmakerconfig.h \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.h \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.h \
    src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget.h \
    src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h \
    src/config/decisions/idecisionmakerconfig.h \
    src/config/decisions/sell/selldecision1config/iselldecision1config.h \
    src/config/decisions/sell/selldecision1config/selldecision1config.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.h \
    src/config/decisions/sell/selldecision2config/iselldecision2config.h \
    src/config/decisions/sell/selldecision2config/selldecision2config.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h \
    src/config/decisions/sell/selldecision3config/iselldecision3config.h \
    src/config/decisions/sell/selldecision3config/selldecision3config.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidgetfactory.h \
    src/config/iconfig.h \
    src/db/stocks/istocksdatabase.h \
    src/db/stocks/stocksdatabase.h \
    src/db/user/iuserdatabase.h \
    src/db/user/userdatabase.h \
    src/dialogs/authdialog/authdialog.h \
    src/dialogs/authdialog/authdialogfactory.h \
    src/dialogs/authdialog/iauthdialog.h \
    src/dialogs/authdialog/iauthdialogfactory.h \
    src/dialogs/orderwavesdialog/iorderwavesdialog.h \
    src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h \
    src/dialogs/orderwavesdialog/orderwavesdialog.h \
    src/dialogs/orderwavesdialog/orderwavesdialogfactory.h \
    src/dialogs/settingsdialog/isettingsdialog.h \
    src/dialogs/settingsdialog/isettingsdialogfactory.h \
    src/dialogs/settingsdialog/settingsdialog.h \
    src/dialogs/settingsdialog/settingsdialogfactory.h \
    src/dialogs/startautopilotdialog/istartautopilotdialog.h \
    src/dialogs/startautopilotdialog/istartautopilotdialogfactory.h \
    src/dialogs/startautopilotdialog/startautopilotdialog.h \
    src/dialogs/startautopilotdialog/startautopilotdialogfactory.h \
    src/dialogs/startsimulationdialog/istartsimulationdialog.h \
    src/dialogs/startsimulationdialog/istartsimulationdialogfactory.h \
    src/dialogs/startsimulationdialog/startsimulationdialog.h \
    src/dialogs/startsimulationdialog/startsimulationdialogfactory.h \
    src/domain/filter/filter.h \
    src/domain/orderbook/orderbook.h \
    src/domain/orderbook/orderbookdata.h \
    src/domain/quotation/quotation.h \
    src/domain/stocks/stock.h \
    src/domain/stocks/stockdata.h \
    src/domain/stocks/stockmeta.h \
    src/domain/stocks/stockoperational.h \
    src/domain/user/account.h \
    src/domain/user/user.h \
    src/grpc/grpcclient.h \
    src/grpc/igrpcclient.h \
    src/grpc/investapiuthenticator.h \
    src/grpc/irawgrpcclient.h \
    src/grpc/rawgrpcclient.h \
    src/grpc/utils.h \
    src/main/mainwindow.h \
    src/storage/stocks/istocksstorage.h \
    src/storage/stocks/stocksstorage.h \
    src/storage/user/iuserstorage.h \
    src/storage/user/userstorage.h \
    src/threads/cleanup/cleanupthread.h \
    src/threads/cleanup/icleanupthread.h \
    src/threads/lastprice/ilastpricethread.h \
    src/threads/lastprice/lastpricethread.h \
    src/threads/makedecision/imakedecisionthread.h \
    src/threads/makedecision/makedecisionthread.h \
    src/threads/orderbook/iorderbookthread.h \
    src/threads/orderbook/orderbookthread.h \
    src/threads/parallelhelper/parallelhelperthread.h \
    src/threads/pricecollect/ipricecollectthread.h \
    src/threads/pricecollect/pricecollectthread.h \
    src/threads/userupdate/iuserupdatethread.h \
    src/threads/userupdate/userupdatethread.h \
    src/utils/autorunenabler/autorunenabler.h \
    src/utils/autorunenabler/iautorunenabler.h \
    src/utils/fs/dir/dir.h \
    src/utils/fs/dir/dirfactory.h \
    src/utils/fs/dir/idir.h \
    src/utils/fs/dir/idirfactory.h \
    src/utils/fs/file/file.h \
    src/utils/fs/file/filefactory.h \
    src/utils/fs/file/ifile.h \
    src/utils/fs/file/ifilefactory.h \
    src/utils/fs/zip/qzip/iqzip.h \
    src/utils/fs/zip/qzip/iqzipfactory.h \
    src/utils/fs/zip/qzip/qzip.h \
    src/utils/fs/zip/qzip/qzipfactory.h \
    src/utils/fs/zip/qzipfile/iqzipfile.h \
    src/utils/fs/zip/qzipfile/iqzipfilefactory.h \
    src/utils/fs/zip/qzipfile/qzipfile.h \
    src/utils/fs/zip/qzipfile/qzipfilefactory.h \
    src/utils/http/httpclient.h \
    src/utils/http/ihttpclient.h \
    src/utils/logger/logger.h \
    src/utils/messagebox/imessagebox.h \
    src/utils/messagebox/messagebox.h \
    src/utils/processrunner/iprocessrunner.h \
    src/utils/processrunner/iprocessrunnerfactory.h \
    src/utils/processrunner/processrunner.h \
    src/utils/processrunner/processrunnerfactory.h \
    src/utils/settingseditor/isettingseditor.h \
    src/utils/settingseditor/settingseditor.h \
    src/utils/style/darkpalette.h \
    src/utils/timeutils/itimeutils.h \
    src/utils/timeutils/timeutils.h \
    src/widgets/accountchartwidget/accountchartwidget.h \
    src/widgets/accountchartwidget/accountchartwidgetfactory.h \
    src/widgets/accountchartwidget/iaccountchartwidget.h \
    src/widgets/accountchartwidget/iaccountchartwidgetfactory.h \
    src/widgets/decisionmakerwidget/decisionmakerwidget.h \
    src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h \
    src/widgets/decisionmakerwidget/idecisionmakerwidget.h \
    src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory.h \
    src/widgets/logstablewidget/ilogstablewidget.h \
    src/widgets/logstablewidget/ilogstablewidgetfactory.h \
    src/widgets/logstablewidget/logstablewidget.h \
    src/widgets/logstablewidget/logstablewidgetfactory.h \
    src/widgets/operationstablewidget/ioperationstablewidget.h \
    src/widgets/operationstablewidget/ioperationstablewidgetfactory.h \
    src/widgets/operationstablewidget/operationstablewidget.h \
    src/widgets/operationstablewidget/operationstablewidgetfactory.h \
    src/widgets/orderwaveswidget/iorderwaveswidget.h \
    src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h \
    src/widgets/orderwaveswidget/orderwaveswidget.h \
    src/widgets/orderwaveswidget/orderwaveswidgetfactory.h \
    src/widgets/portfoliotablewidget/iportfoliotablewidget.h \
    src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory.h \
    src/widgets/portfoliotablewidget/portfoliotablewidget.h \
    src/widgets/portfoliotablewidget/portfoliotablewidgetfactory.h \
    src/widgets/stockscontrolswidget/istockscontrolswidget.h \
    src/widgets/stockscontrolswidget/istockscontrolswidgetfactory.h \
    src/widgets/stockscontrolswidget/stockscontrolswidget.h \
    src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h \
    src/widgets/stockstablewidget/istockstablewidget.h \
    src/widgets/stockstablewidget/istockstablewidgetfactory.h \
    src/widgets/stockstablewidget/stockstablewidget.h \
    src/widgets/stockstablewidget/stockstablewidgetfactory.h \
    src/widgets/tableitems/actions/actionstableitemwidget.h \
    src/widgets/tableitems/actions/actionstableitemwidgetfactory.h \
    src/widgets/tableitems/actions/iactionstableitemwidget.h \
    src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h \
    src/widgets/tableitems/paybacktableitem.h \
    src/widgets/tableitems/pricechangetableitem.h \
    src/widgets/tableitems/pricetableitem.h \
    src/widgets/tableitems/stock/istocktableitemwidget.h \
    src/widgets/tableitems/stock/istocktableitemwidgetfactory.h \
    src/widgets/tableitems/stock/stocktableitemwidget.h \
    src/widgets/tableitems/stock/stocktableitemwidgetfactory.h \
    src/widgets/tableitems/turnovertableitem.h \
    src/widgets/tablerecords/stockstablerecord/istockstablerecord.h \
    src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory.h \
    src/widgets/tablerecords/stockstablerecord/stockstablerecord.h \
    src/widgets/tablerecords/stockstablerecord/stockstablerecordfactory.h \
    src/widgets/trayicon/itrayicon.h \
    src/widgets/trayicon/itrayiconfactory.h \
    src/widgets/trayicon/trayicon.h \
    src/widgets/trayicon/trayiconfactory.h \

# find app/ -name \*.ui | sort | sed "s/app\///g" | sed "s/\.ui/\.ui \\\/g"
FORMS += \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.ui \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.ui \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.ui \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.ui \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.ui \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.ui \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.ui \
    src/dialogs/authdialog/authdialog.ui \
    src/dialogs/orderwavesdialog/orderwavesdialog.ui \
    src/dialogs/settingsdialog/settingsdialog.ui \
    src/dialogs/startautopilotdialog/startautopilotdialog.ui \
    src/dialogs/startsimulationdialog/startsimulationdialog.ui \
    src/main/mainwindow.ui \
    src/widgets/decisionmakerwidget/decisionmakerwidget.ui \
    src/widgets/logstablewidget/logstablewidget.ui \
    src/widgets/operationstablewidget/operationstablewidget.ui \
    src/widgets/portfoliotablewidget/portfoliotablewidget.ui \
    src/widgets/stockscontrolswidget/stockscontrolswidget.ui \
    src/widgets/stockstablewidget/stockstablewidget.ui \
    src/widgets/tableitems/actions/actionstableitemwidget.ui \
    src/widgets/tableitems/stock/stocktableitemwidget.ui \
