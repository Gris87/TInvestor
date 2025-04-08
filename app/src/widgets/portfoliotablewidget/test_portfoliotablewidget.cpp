#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"
#include "ui_portfoliotablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



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
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),          QVariant(60))).WillOnce(Return(QVariant(60)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Available"),     QVariant(58))).WillOnce(Return(QVariant(58)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),         QVariant(34))).WillOnce(Return(QVariant(34)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPrice"),      QVariant(84))).WillOnce(Return(QVariant(84)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Cost"),          QVariant(65))).WillOnce(Return(QVariant(65)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Part"),          QVariant(34))).WillOnce(Return(QVariant(34)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Income"),        QVariant(41))).WillOnce(Return(QVariant(41)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_IncomePercent"), QVariant(57))).WillOnce(Return(QVariant(57)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_InADay"),        QVariant(64))).WillOnce(Return(QVariant(64)));
    // clang-format on

    portfolioTableWidget->loadWindowState("AAAAA");
}
