#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidget_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidget_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidget_mock.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory_mock.h"



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

    StrictMock<OperationsTableWidgetFactoryMock> operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>    accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTableWidgetFactoryMock>  portfolioTableWidgetFactoryMock;
    StrictMock<OperationsTableRecordFactoryMock> operationsTableRecordFactoryMock;
    StrictMock<StockTableItemWidgetFactoryMock>  stockTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                  userStorageMock;
    StrictMock<InstrumentsStorageMock>           instrumentsStorageMock;
    StrictMock<SettingsEditorMock>               settingsEditorMock;

    // It will be deleted by `delete ui;`
    StrictMock<OperationsTableWidgetMock>* operationsTableWidgetMock = new StrictMock<OperationsTableWidgetMock>();
    StrictMock<AccountChartWidgetMock>*    accountChartWidgetMock    = new StrictMock<AccountChartWidgetMock>();
    StrictMock<LogsTableWidgetMock>*       logsTableWidgetMock       = new StrictMock<LogsTableWidgetMock>();
    StrictMock<PortfolioTableWidgetMock>*  portfolioTableWidgetMock  = new StrictMock<PortfolioTableWidgetMock>();

    EXPECT_CALL(
        operationsTableWidgetFactoryMock,
        newInstance(
            &operationsTableRecordFactoryMock,
            &stockTableItemWidgetFactoryMock,
            &userStorageMock,
            &instrumentsStorageMock,
            &settingsEditorMock,
            NotNull()
        )
    )
        .WillOnce(Return(operationsTableWidgetMock));
    EXPECT_CALL(accountChartWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(accountChartWidgetMock));
    EXPECT_CALL(logsTableWidgetFactoryMock, newInstance(&settingsEditorMock, NotNull())).WillOnce(Return(logsTableWidgetMock));
    EXPECT_CALL(portfolioTableWidgetFactoryMock, newInstance(&settingsEditorMock, NotNull()))
        .WillOnce(Return(portfolioTableWidgetMock));

    const IDecisionMakerWidget* widget = factory.newInstance(
        &operationsTableWidgetFactoryMock,
        &accountChartWidgetFactoryMock,
        &logsTableWidgetFactoryMock,
        &portfolioTableWidgetFactoryMock,
        &operationsTableRecordFactoryMock,
        &stockTableItemWidgetFactoryMock,
        &userStorageMock,
        &instrumentsStorageMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
