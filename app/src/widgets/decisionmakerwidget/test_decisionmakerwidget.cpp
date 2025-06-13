#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidget_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidget_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidget_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"
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
        logsFilterWidgetFactoryMock          = new StrictMock<LogsFilterWidgetFactoryMock>();
        logsTableWidgetFactoryMock           = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTreeWidgetFactoryMock       = new StrictMock<PortfolioTreeWidgetFactoryMock>();
        operationsTableWidgetMock            = new StrictMock<OperationsTableWidgetMock>();
        accountChartWidgetMock               = new StrictMock<AccountChartWidgetMock>();
        logsFilterWidgetMock                 = new StrictMock<LogsFilterWidgetMock>();
        logsTableWidgetMock                  = new StrictMock<LogsTableWidgetMock>();
        portfolioTreeWidgetMock              = new StrictMock<PortfolioTreeWidgetMock>();
        operationsTableRecordFactoryMock     = new StrictMock<OperationsTableRecordFactoryMock>();
        logsTableModelFactoryMock            = new StrictMock<LogsTableModelFactoryMock>();
        logsTableRecordFactoryMock           = new StrictMock<LogsTableRecordFactoryMock>();
        portfolioTreeRecordFactoryMock       = new StrictMock<PortfolioTreeRecordFactoryMock>();
        instrumentWidgetFactoryMock          = new StrictMock<InstrumentWidgetFactoryMock>();
        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        logLevelTableItemWidgetFactoryMock   = new StrictMock<LogLevelTableItemWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock                     = new StrictMock<LogosStorageMock>();
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
        EXPECT_CALL(*logsFilterWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(logsFilterWidgetMock));
        EXPECT_CALL(
            *logsTableWidgetFactoryMock,
            newInstance(
                logsTableModelFactoryMock,
                logsTableRecordFactoryMock,
                logLevelTableItemWidgetFactoryMock,
                instrumentTableItemWidgetFactoryMock,
                userStorageMock,
                instrumentsStorageMock,
                logosStorageMock,
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
            logsFilterWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            portfolioTreeWidgetFactoryMock,
            operationsTableRecordFactoryMock,
            logsTableModelFactoryMock,
            logsTableRecordFactoryMock,
            portfolioTreeRecordFactoryMock,
            instrumentWidgetFactoryMock,
            instrumentTableItemWidgetFactoryMock,
            logLevelTableItemWidgetFactoryMock,
            userStorageMock,
            instrumentsStorageMock,
            logosStorageMock,
            fileDialogFactoryMock,
            settingsEditorMock
        );
    }

    void TearDown() override
    {
        delete decisionMakerWidget;
        delete operationsTableWidgetFactoryMock;
        delete accountChartWidgetFactoryMock;
        delete logsFilterWidgetFactoryMock;
        delete logsTableWidgetFactoryMock;
        delete portfolioTreeWidgetFactoryMock;
        // It will be deleted by `delete ui;`
        /*
        delete operationsTableWidgetMock;
        delete accountChartWidgetMock;
        delete logsFilterWidgetMock;
        delete logsTableWidgetMock;
        delete portfolioTreeWidgetMock;
        */
        delete operationsTableRecordFactoryMock;
        delete logsTableModelFactoryMock;
        delete logsTableRecordFactoryMock;
        delete portfolioTreeRecordFactoryMock;
        delete instrumentWidgetFactoryMock;
        delete instrumentTableItemWidgetFactoryMock;
        delete logLevelTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
    }

    DecisionMakerWidget*                              decisionMakerWidget;
    StrictMock<OperationsTableWidgetFactoryMock>*     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*        accountChartWidgetFactoryMock;
    StrictMock<LogsFilterWidgetFactoryMock>*          logsFilterWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*           logsTableWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>*       portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableWidgetMock>*            operationsTableWidgetMock;
    StrictMock<AccountChartWidgetMock>*               accountChartWidgetMock;
    StrictMock<LogsFilterWidgetMock>*                 logsFilterWidgetMock;
    StrictMock<LogsTableWidgetMock>*                  logsTableWidgetMock;
    StrictMock<PortfolioTreeWidgetMock>*              portfolioTreeWidgetMock;
    StrictMock<OperationsTableRecordFactoryMock>*     operationsTableRecordFactoryMock;
    StrictMock<LogsTableModelFactoryMock>*            logsTableModelFactoryMock;
    StrictMock<LogsTableRecordFactoryMock>*           logsTableRecordFactoryMock;
    StrictMock<PortfolioTreeRecordFactoryMock>*       portfolioTreeRecordFactoryMock;
    StrictMock<InstrumentWidgetFactoryMock>*          instrumentWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<LogLevelTableItemWidgetFactoryMock>*   logLevelTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<LogosStorageMock>*                     logosStorageMock;
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
