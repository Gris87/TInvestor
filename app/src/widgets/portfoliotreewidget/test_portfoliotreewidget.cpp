#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodel_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_PortfolioTreeWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        portfolioTreeModelFactoryMock = new StrictMock<PortfolioTreeModelFactoryMock>();
        fileDialogFactoryMock         = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock            = new StrictMock<SettingsEditorMock>();

        portfolioTreeModelMock = new StrictMock<PortfolioTreeModelMock>();

        EXPECT_CALL(*portfolioTreeModelFactoryMock, newInstance(NotNull())).WillOnce(Return(portfolioTreeModelMock));
        EXPECT_CALL(*portfolioTreeModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
        EXPECT_CALL(*portfolioTreeModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

        portfolioTreeWidget = new PortfolioTreeWidget(portfolioTreeModelFactoryMock, fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete portfolioTreeWidget;
        delete portfolioTreeModelFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete portfolioTreeModelMock;
    }

    PortfolioTreeWidget*                       portfolioTreeWidget;
    StrictMock<PortfolioTreeModelFactoryMock>* portfolioTreeModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*         fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*            settingsEditorMock;
    StrictMock<PortfolioTreeModelMock>*        portfolioTreeModelMock;
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
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),         _)).WillOnce(Return(QVariant(172)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Available"),    _)).WillOnce(Return(QVariant(80)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),        _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPrice"),     _)).WillOnce(Return(QVariant(124)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Cost"),         _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Part"),         _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Yield"),        _)).WillOnce(Return(QVariant(68)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldPercent"), _)).WillOnce(Return(QVariant(79)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_DailyYield"),   _)).WillOnce(Return(QVariant(1100)));
    // clang-format on

    portfolioTreeWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
