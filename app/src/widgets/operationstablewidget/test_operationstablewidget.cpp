#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
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
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();

        operationsTableWidget = new OperationsTableWidget(
            operationsTableRecordFactoryMock,
            instrumentTableItemWidgetFactoryMock,
            userStorageMock,
            instrumentsStorageMock,
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
        delete settingsEditorMock;
    }

    OperationsTableWidget*                            operationsTableWidget;
    StrictMock<OperationsTableRecordFactoryMock>*     operationsTableRecordFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
};



TEST_F(Test_OperationsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Description"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),       _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Quantity"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Payment"),     _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Commission"),  _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Yield"),       _));
    // clang-format on

    operationsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_OperationsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),        _)).WillOnce(Return(QVariant(64)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),        _)).WillOnce(Return(QVariant(82)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Description"), _)).WillOnce(Return(QVariant(85)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),       _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Quantity"),    _)).WillOnce(Return(QVariant(95)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Payment"),     _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Commission"),  _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Yield"),       _)).WillOnce(Return(QVariant(87)));
    // clang-format on

    operationsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
