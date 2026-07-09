#include "src/widgets/notificationstablewidget/notificationstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel_mock.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_NotificationsTableWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new NotificationsTableWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    NotificationsTableWidgetFactory* factory;
};



TEST_F(Test_NotificationsTableWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsTableWidgetFactory, Test_newInstance)
{
    // const InSequence seq;

    StrictMock<NotificationsTableModelFactoryMock> notificationsTableModelFactoryMock;
    StrictMock<HttpClientMock>                     httpClientMock;
    StrictMock<SettingsEditorMock>                 settingsEditorMock;

    StrictMock<NotificationsTableModelMock> notificationsTableModelMock;

    EXPECT_CALL(notificationsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(&notificationsTableModelMock));
    EXPECT_CALL(notificationsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
    EXPECT_CALL(notificationsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

    const INotificationsTableWidget* widget =
        factory->newInstance(&notificationsTableModelFactoryMock, &httpClientMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
