#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_OperationsTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        operationsTableRecordFactoryMock     = new StrictMock<OperationsTableRecordFactoryMock>();
        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        fileDialogFactoryMock                = new StrictMock<FileDialogFactoryMock>();
        messageBoxUtilsMock                  = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();

        operationsTableWidget = new OperationsTableWidget(
            operationsTableRecordFactoryMock,
            instrumentTableItemWidgetFactoryMock,
            userStorageMock,
            instrumentsStorageMock,
            fileDialogFactoryMock,
            messageBoxUtilsMock,
            settingsEditorMock
        );
    }

    void TearDown() override
    {
        delete operationsTableWidget;
        delete operationsTableRecordFactoryMock;
        delete instrumentTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        delete fileDialogFactoryMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
    }

    OperationsTableWidget*                            operationsTableWidget;
    StrictMock<OperationsTableRecordFactoryMock>*     operationsTableRecordFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<FileDialogFactoryMock>*                fileDialogFactoryMock;
    StrictMock<MessageBoxUtilsMock>*                  messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
};



TEST_F(Test_OperationsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"),                       _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),                       _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Description"),                _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),                      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_AvgPrice"),                   _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Quantity"),                   _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_RemainedQuantity"),           _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Payment"),                    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Commission"),                 _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Yield"),                      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_YieldWithCommission"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_YieldWithCommissionPercent"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_RemainedMoney"),              _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_TotalMoney"),                 _));
    // clang-format on

    operationsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_OperationsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),                       _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),                       _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Description"),                _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),                      _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPrice"),                   _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Quantity"),                   _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_RemainedQuantity"),           _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Payment"),                    _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Commission"),                 _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Yield"),                      _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldWithCommission"),        _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldWithCommissionPercent"), _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_RemainedMoney"),              _)).WillOnce(Return(QVariant(10)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_TotalMoney"),                 _)).WillOnce(Return(QVariant(10)));
    // clang-format on

    operationsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
