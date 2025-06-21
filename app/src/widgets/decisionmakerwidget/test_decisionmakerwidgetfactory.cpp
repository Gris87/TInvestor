#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidget_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidget_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidget_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
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
    StrictMock<LogsFilterWidgetFactoryMock>      logsFilterWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>   portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableModelFactoryMock>  operationsTableModelFactoryMock;
    StrictMock<LogsTableModelFactoryMock>        logsTableModelFactoryMock;
    StrictMock<PortfolioTreeModelFactoryMock>    portfolioTreeModelFactoryMock;
    StrictMock<LogosStorageMock>                 logosStorageMock;
    StrictMock<FileDialogFactoryMock>            fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>               settingsEditorMock;

    // It will be deleted by `delete ui;`
    StrictMock<OperationsTableWidgetMock>* operationsTableWidgetMock = new StrictMock<OperationsTableWidgetMock>();
    StrictMock<AccountChartWidgetMock>*    accountChartWidgetMock    = new StrictMock<AccountChartWidgetMock>();
    StrictMock<LogsFilterWidgetMock>*      logsFilterWidgetMock      = new StrictMock<LogsFilterWidgetMock>();
    StrictMock<LogsTableWidgetMock>*       logsTableWidgetMock       = new StrictMock<LogsTableWidgetMock>();
    StrictMock<PortfolioTreeWidgetMock>*   portfolioTreeWidgetMock   = new StrictMock<PortfolioTreeWidgetMock>();

    LogFilter filter;

    EXPECT_CALL(
        operationsTableWidgetFactoryMock,
        newInstance(&operationsTableModelFactoryMock, &logosStorageMock, &fileDialogFactoryMock, &settingsEditorMock, NotNull())
    )
        .WillOnce(Return(operationsTableWidgetMock));
    EXPECT_CALL(accountChartWidgetFactoryMock, newInstance(&fileDialogFactoryMock, &settingsEditorMock, NotNull()))
        .WillOnce(Return(accountChartWidgetMock));
    EXPECT_CALL(logsFilterWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(logsFilterWidgetMock));
    EXPECT_CALL(
        logsTableWidgetFactoryMock,
        newInstance(&logsTableModelFactoryMock, &logosStorageMock, &fileDialogFactoryMock, &settingsEditorMock, NotNull())
    )
        .WillOnce(Return(logsTableWidgetMock));
    EXPECT_CALL(
        portfolioTreeWidgetFactoryMock,
        newInstance(&portfolioTreeModelFactoryMock, &logosStorageMock, &fileDialogFactoryMock, &settingsEditorMock, NotNull())
    )
        .WillOnce(Return(portfolioTreeWidgetMock));

    EXPECT_CALL(*logsFilterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*logsTableWidgetMock, setFilter(filter));

    const IDecisionMakerWidget* widget = factory.newInstance(
        &operationsTableWidgetFactoryMock,
        &accountChartWidgetFactoryMock,
        &logsFilterWidgetFactoryMock,
        &logsTableWidgetFactoryMock,
        &portfolioTreeWidgetFactoryMock,
        &operationsTableModelFactoryMock,
        &logsTableModelFactoryMock,
        &portfolioTreeModelFactoryMock,
        &logosStorageMock,
        &fileDialogFactoryMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
