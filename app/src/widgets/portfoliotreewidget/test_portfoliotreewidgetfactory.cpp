#include "src/widgets/portfoliotreewidget/portfoliotreewidgetfactory.h"

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



class Test_PortfolioTreeWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new PortfolioTreeWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    PortfolioTreeWidgetFactory* factory;
};



TEST_F(Test_PortfolioTreeWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioTreeWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<PortfolioTreeModelFactoryMock> portfolioTreeModelFactoryMock;
    StrictMock<FileDialogFactoryMock>         fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>            settingsEditorMock;

    StrictMock<PortfolioTreeModelMock> portfolioTreeModelMock;

    EXPECT_CALL(portfolioTreeModelFactoryMock, newInstance(NotNull())).WillOnce(Return(&portfolioTreeModelMock));
    EXPECT_CALL(portfolioTreeModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
    EXPECT_CALL(portfolioTreeModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

    const IPortfolioTreeWidget* widget =
        factory->newInstance(&portfolioTreeModelFactoryMock, &fileDialogFactoryMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
