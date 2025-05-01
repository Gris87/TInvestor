#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"
#include "ui_portfoliotablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_PortfolioTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        portfolioTableWidget = new PortfolioTableWidget(settingsEditorMock);
    }

    void TearDown() override
    {
        delete portfolioTableWidget;
        delete settingsEditorMock;
    }

    PortfolioTableWidget*           portfolioTableWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_PortfolioTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),          _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Available"),     _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_AvgPrice"),      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Cost"),          _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Part"),          _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Income"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_IncomePercent"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_InADay"),        _));
    // clang-format on

    portfolioTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_PortfolioTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),          _)).WillOnce(Return(QVariant(82)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Available"),     _)).WillOnce(Return(QVariant(80)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),         _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPrice"),      _)).WillOnce(Return(QVariant(106)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Cost"),          _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Part"),          _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Income"),        _)).WillOnce(Return(QVariant(63)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_IncomePercent"), _)).WillOnce(Return(QVariant(79)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_InADay"),        _)).WillOnce(Return(QVariant(86)));
    // clang-format on

    portfolioTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
