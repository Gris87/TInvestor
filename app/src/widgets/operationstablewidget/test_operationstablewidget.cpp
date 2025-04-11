#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



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
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        operationsTableWidget = new OperationsTableWidget(settingsEditorMock);
    }

    void TearDown() override
    {
        delete operationsTableWidget;
        delete settingsEditorMock;
    }

    OperationsTableWidget*          operationsTableWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_OperationsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"),      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Operation"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),     _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Quantity"),  _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Cost"),      _));
    // clang-format on

    operationsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_OperationsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),      QVariant(42))).WillOnce(Return(QVariant(42)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),      QVariant(60))).WillOnce(Return(QVariant(60)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Operation"), QVariant(63))).WillOnce(Return(QVariant(63)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),     QVariant(34))).WillOnce(Return(QVariant(34)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Quantity"),  QVariant(73))).WillOnce(Return(QVariant(73)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Cost"),      QVariant(65))).WillOnce(Return(QVariant(65)));
    // clang-format on

    operationsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
