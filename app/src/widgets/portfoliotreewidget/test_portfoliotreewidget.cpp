#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_PortfolioTreeWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        portfolioTreeRecordFactoryMock = new StrictMock<PortfolioTreeRecordFactoryMock>();
        instrumentsStorageMock         = new StrictMock<InstrumentsStorageMock>();
        settingsEditorMock             = new StrictMock<SettingsEditorMock>();

        portfolioTreeWidget = new PortfolioTreeWidget(portfolioTreeRecordFactoryMock, instrumentsStorageMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete portfolioTreeWidget;
        delete portfolioTreeRecordFactoryMock;
        delete instrumentsStorageMock;
        delete settingsEditorMock;
    }

    PortfolioTreeWidget*                        portfolioTreeWidget;
    StrictMock<PortfolioTreeRecordFactoryMock>* portfolioTreeRecordFactoryMock;
    StrictMock<InstrumentsStorageMock>*         instrumentsStorageMock;
    StrictMock<SettingsEditorMock>*             settingsEditorMock;
};



TEST_F(Test_PortfolioTreeWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioTreeWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Available"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_AvgPrice"),     _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Cost"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Part"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Yield"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_YieldPercent"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_DailyYield"),   _));
    // clang-format on

    portfolioTreeWidget->saveWindowState("AAAAA");
}

TEST_F(Test_PortfolioTreeWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),         _)).WillOnce(Return(QVariant(82)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Available"),    _)).WillOnce(Return(QVariant(80)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),        _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPrice"),     _)).WillOnce(Return(QVariant(106)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Cost"),         _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Part"),         _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Yield"),        _)).WillOnce(Return(QVariant(63)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldPercent"), _)).WillOnce(Return(QVariant(79)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_DailyYield"),   _)).WillOnce(Return(QVariant(86)));
    // clang-format on

    portfolioTreeWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
