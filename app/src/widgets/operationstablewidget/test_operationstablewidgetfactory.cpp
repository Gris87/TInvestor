#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_OperationsTableWidgetFactory, Test_constructor_and_destructor)
{
    const OperationsTableWidgetFactory factory;
}

TEST(Test_OperationsTableWidgetFactory, Test_newInstance)
{
    const OperationsTableWidgetFactory factory;

    StrictMock<OperationsTableRecordFactoryMock>     operationsTableRecordFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock> instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                      userStorageMock;
    StrictMock<InstrumentsStorageMock>               instrumentsStorageMock;
    StrictMock<SettingsEditorMock>                   settingsEditorMock;

    const IOperationsTableWidget* widget = factory.newInstance(
        &operationsTableRecordFactoryMock,
        &instrumentTableItemWidgetFactoryMock,
        &userStorageMock,
        &instrumentsStorageMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
