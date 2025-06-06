#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidget_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidget_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory_mock.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory_mock.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_DecisionMakerWidgetFactory, Test_constructor_and_destructor)
{
    const DecisionMakerWidgetFactory factory;
}

TEST(Test_DecisionMakerWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    const DecisionMakerWidgetFactory factory;

    StrictMock<OperationsTableWidgetFactoryMock>     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>        accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>           logsTableWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>       portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableRecordFactoryMock>     operationsTableRecordFactoryMock;
    StrictMock<LogsTableRecordFactoryMock>           logsTableRecordFactoryMock;
    StrictMock<PortfolioTreeRecordFactoryMock>       portfolioTreeRecordFactoryMock;
    StrictMock<InstrumentWidgetFactoryMock>          instrumentWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock> instrumentTableItemWidgetFactoryMock;
    StrictMock<LogLevelTableItemWidgetFactoryMock>   logLevelTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                      userStorageMock;
    StrictMock<InstrumentsStorageMock>               instrumentsStorageMock;
    StrictMock<FileDialogFactoryMock>                fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>                   settingsEditorMock;

    // It will be deleted by `delete ui;`
    StrictMock<OperationsTableWidgetMock>* operationsTableWidgetMock = new StrictMock<OperationsTableWidgetMock>();
    StrictMock<AccountChartWidgetMock>*    accountChartWidgetMock    = new StrictMock<AccountChartWidgetMock>();
    StrictMock<LogsTableWidgetMock>*       logsTableWidgetMock       = new StrictMock<LogsTableWidgetMock>();
    StrictMock<PortfolioTreeWidgetMock>*   portfolioTreeWidgetMock   = new StrictMock<PortfolioTreeWidgetMock>();

    EXPECT_CALL(
        operationsTableWidgetFactoryMock,
        newInstance(
            &operationsTableRecordFactoryMock,
            &instrumentTableItemWidgetFactoryMock,
            &userStorageMock,
            &instrumentsStorageMock,
            &fileDialogFactoryMock,
            &settingsEditorMock,
            NotNull()
        )
    )
        .WillOnce(Return(operationsTableWidgetMock));
    EXPECT_CALL(accountChartWidgetFactoryMock, newInstance(&fileDialogFactoryMock, &settingsEditorMock, NotNull()))
        .WillOnce(Return(accountChartWidgetMock));
    EXPECT_CALL(
        logsTableWidgetFactoryMock,
        newInstance(
            &logsTableRecordFactoryMock,
            &logLevelTableItemWidgetFactoryMock,
            &fileDialogFactoryMock,
            &settingsEditorMock,
            NotNull()
        )
    )
        .WillOnce(Return(logsTableWidgetMock));
    EXPECT_CALL(
        portfolioTreeWidgetFactoryMock,
        newInstance(
            &portfolioTreeRecordFactoryMock,
            &instrumentWidgetFactoryMock,
            &userStorageMock,
            &instrumentsStorageMock,
            &fileDialogFactoryMock,
            &settingsEditorMock,
            NotNull()
        )
    )
        .WillOnce(Return(portfolioTreeWidgetMock));

    const IDecisionMakerWidget* widget = factory.newInstance(
        &operationsTableWidgetFactoryMock,
        &accountChartWidgetFactoryMock,
        &logsTableWidgetFactoryMock,
        &portfolioTreeWidgetFactoryMock,
        &operationsTableRecordFactoryMock,
        &logsTableRecordFactoryMock,
        &portfolioTreeRecordFactoryMock,
        &instrumentWidgetFactoryMock,
        &instrumentTableItemWidgetFactoryMock,
        &logLevelTableItemWidgetFactoryMock,
        &userStorageMock,
        &instrumentsStorageMock,
        &fileDialogFactoryMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
