#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodel_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OperationsTableWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new OperationsTableWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    OperationsTableWidgetFactory* factory;
};



TEST_F(Test_OperationsTableWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<OperationsTableModelFactoryMock> operationsTableModelFactoryMock;
    StrictMock<FileDialogFactoryMock>           fileDialogFactoryMock;
    StrictMock<ConfigMock>                      configMock;
    StrictMock<SettingsEditorMock>              settingsEditorMock;

    StrictMock<OperationsTableModelMock> operationsTableModelMock;

    EXPECT_CALL(operationsTableModelFactoryMock, newInstance(&configMock, NotNull())).WillOnce(Return(&operationsTableModelMock));
    EXPECT_CALL(operationsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
    EXPECT_CALL(operationsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

    const IOperationsTableWidget* widget =
        factory->newInstance(&operationsTableModelFactoryMock, &fileDialogFactoryMock, &configMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
