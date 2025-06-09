#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

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



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_DecisionMakerWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        operationsTableWidgetFactoryMock     = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock        = new StrictMock<AccountChartWidgetFactoryMock>();
        logsTableWidgetFactoryMock           = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTreeWidgetFactoryMock       = new StrictMock<PortfolioTreeWidgetFactoryMock>();
        operationsTableWidgetMock            = new StrictMock<OperationsTableWidgetMock>();
        accountChartWidgetMock               = new StrictMock<AccountChartWidgetMock>();
        logsTableWidgetMock                  = new StrictMock<LogsTableWidgetMock>();
        portfolioTreeWidgetMock              = new StrictMock<PortfolioTreeWidgetMock>();
        operationsTableRecordFactoryMock     = new StrictMock<OperationsTableRecordFactoryMock>();
        logsTableRecordFactoryMock           = new StrictMock<LogsTableRecordFactoryMock>();
        portfolioTreeRecordFactoryMock       = new StrictMock<PortfolioTreeRecordFactoryMock>();
        instrumentWidgetFactoryMock          = new StrictMock<InstrumentWidgetFactoryMock>();
        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        logLevelTableItemWidgetFactoryMock   = new StrictMock<LogLevelTableItemWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        fileDialogFactoryMock                = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();

        EXPECT_CALL(
            *operationsTableWidgetFactoryMock,
            newInstance(
                operationsTableRecordFactoryMock,
                instrumentTableItemWidgetFactoryMock,
                userStorageMock,
                instrumentsStorageMock,
                fileDialogFactoryMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(operationsTableWidgetMock));
        EXPECT_CALL(*accountChartWidgetFactoryMock, newInstance(fileDialogFactoryMock, settingsEditorMock, NotNull()))
            .WillOnce(Return(accountChartWidgetMock));
        EXPECT_CALL(
            *logsTableWidgetFactoryMock,
            newInstance(
                logsTableRecordFactoryMock,
                logLevelTableItemWidgetFactoryMock,
                instrumentTableItemWidgetFactoryMock,
                userStorageMock,
                instrumentsStorageMock,
                fileDialogFactoryMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(logsTableWidgetMock));
        EXPECT_CALL(
            *portfolioTreeWidgetFactoryMock,
            newInstance(
                portfolioTreeRecordFactoryMock,
                instrumentWidgetFactoryMock,
                userStorageMock,
                instrumentsStorageMock,
                fileDialogFactoryMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(portfolioTreeWidgetMock));

        decisionMakerWidget = new DecisionMakerWidget(
            operationsTableWidgetFactoryMock,
            accountChartWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            portfolioTreeWidgetFactoryMock,
            operationsTableRecordFactoryMock,
            logsTableRecordFactoryMock,
            portfolioTreeRecordFactoryMock,
            instrumentWidgetFactoryMock,
            instrumentTableItemWidgetFactoryMock,
            logLevelTableItemWidgetFactoryMock,
            userStorageMock,
            instrumentsStorageMock,
            fileDialogFactoryMock,
            settingsEditorMock
        );
    }

    void TearDown() override
    {
        delete decisionMakerWidget;
        delete operationsTableWidgetFactoryMock;
        delete accountChartWidgetFactoryMock;
        delete logsTableWidgetFactoryMock;
        delete portfolioTreeWidgetFactoryMock;
        delete operationsTableRecordFactoryMock;
        delete logsTableRecordFactoryMock;
        delete portfolioTreeRecordFactoryMock;
        delete instrumentWidgetFactoryMock;
        delete instrumentTableItemWidgetFactoryMock;
        delete logLevelTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
    }

    DecisionMakerWidget*                              decisionMakerWidget;
    StrictMock<OperationsTableWidgetFactoryMock>*     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*        accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*           logsTableWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>*       portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableWidgetMock>*            operationsTableWidgetMock;
    StrictMock<AccountChartWidgetMock>*               accountChartWidgetMock;
    StrictMock<LogsTableWidgetMock>*                  logsTableWidgetMock;
    StrictMock<PortfolioTreeWidgetMock>*              portfolioTreeWidgetMock;
    StrictMock<OperationsTableRecordFactoryMock>*     operationsTableRecordFactoryMock;
    StrictMock<LogsTableRecordFactoryMock>*           logsTableRecordFactoryMock;
    StrictMock<PortfolioTreeRecordFactoryMock>*       portfolioTreeRecordFactoryMock;
    StrictMock<InstrumentWidgetFactoryMock>*          instrumentWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<LogLevelTableItemWidgetFactoryMock>*   logLevelTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<FileDialogFactoryMock>*                fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
};



TEST_F(Test_DecisionMakerWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMakerWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock,        setValue(QString("AAAAA/splitter"), _));
    EXPECT_CALL(*operationsTableWidgetMock, saveWindowState(QString("AAAAA/OperationsTableWidget")));
    EXPECT_CALL(*logsTableWidgetMock,       saveWindowState(QString("AAAAA/LogsTableWidget")));
    EXPECT_CALL(*portfolioTreeWidgetMock,   saveWindowState(QString("AAAAA/PortfolioTreeWidget")));
    // clang-format on

    decisionMakerWidget->saveWindowState("AAAAA");
}

TEST_F(Test_DecisionMakerWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock,        value(QString("AAAAA/splitter"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
    EXPECT_CALL(*operationsTableWidgetMock, loadWindowState(QString("AAAAA/OperationsTableWidget")));
    EXPECT_CALL(*logsTableWidgetMock,       loadWindowState(QString("AAAAA/LogsTableWidget")));
    EXPECT_CALL(*portfolioTreeWidgetMock,   loadWindowState(QString("AAAAA/PortfolioTreeWidget")));
    // clang-format on

    decisionMakerWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
