#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"

#include <gtest/gtest.h>

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



TEST(Test_OperationsTableWidgetFactory, Test_constructor_and_destructor)
{
    const OperationsTableWidgetFactory factory;
}

TEST(Test_OperationsTableWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    const OperationsTableWidgetFactory factory;

    StrictMock<OperationsTableModelFactoryMock> operationsTableModelFactoryMock;
    StrictMock<FileDialogFactoryMock>           fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>              settingsEditorMock;

    StrictMock<OperationsTableModelMock> operationsTableModelMock;

    EXPECT_CALL(operationsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(&operationsTableModelMock));
    EXPECT_CALL(operationsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
    EXPECT_CALL(operationsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

    const IOperationsTableWidget* widget =
        factory.newInstance(&operationsTableModelFactoryMock, &fileDialogFactoryMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
