#include "src/domain/operation/operationfifoitem.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_OperationFifoItem, Test_constructor_and_destructor)
{
    const OperationFifoItem item;

    // clang-format off
    ASSERT_EQ(item.quantity,   0);
    ASSERT_EQ(item.cost.units, 0);
    ASSERT_EQ(item.cost.nano,  0);
    // clang-format on
}

TEST(Test_OperationFifoItem, Test_copy_constructor)
{
    OperationFifoItem item;

    item.quantity   = 1;
    item.cost.units = 2;
    item.cost.nano  = 3;

    const OperationFifoItem item2(item);

    // clang-format off
    ASSERT_EQ(item2.quantity,   1);
    ASSERT_EQ(item2.cost.units, 2);
    ASSERT_EQ(item2.cost.nano,  3);
    // clang-format on
}

TEST(Test_OperationFifoItem, Test_assign)
{
    OperationFifoItem item;
    OperationFifoItem item2;

    item.quantity   = 1;
    item.cost.units = 2;
    item.cost.nano  = 3;

    item2 = item;

    // clang-format off
    ASSERT_EQ(item2.quantity,   1);
    ASSERT_EQ(item2.cost.units, 2);
    ASSERT_EQ(item2.cost.nano,  3);
    // clang-format on
}

TEST(Test_OperationFifoItem, Test_fromJsonObject)
{
    OperationFifoItem item;

    // clang-format off
    ASSERT_EQ(item.quantity,   0);
    ASSERT_EQ(item.cost.units, 0);
    ASSERT_EQ(item.cost.nano,  0);
    // clang-format on

    const QString content = R"({"cost":{"nano":3,"units":2},"quantity":1})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    item.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(item.quantity,   1);
    ASSERT_EQ(item.cost.units, 2);
    ASSERT_EQ(item.cost.nano,  3);
    // clang-format on
}

TEST(Test_OperationFifoItem, Test_toJsonObject)
{
    OperationFifoItem item;

    item.quantity   = 1;
    item.cost.units = 2;
    item.cost.nano  = 3;

    const QJsonObject   jsonObject = item.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"cost":{"nano":3,"units":2},"quantity":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_OperationFifoItem, Test_equals)
{
    OperationFifoItem item;
    OperationFifoItem item2;

    item.quantity   = 1;
    item.cost.units = 2;
    item.cost.nano  = 3;

    item2.quantity   = 1;
    item2.cost.units = 2;
    item2.cost.nano  = 3;

    ASSERT_EQ(item, item2);

    item2.quantity = 1111;
    ASSERT_NE(item, item2);
    item2.quantity = 1;
    ASSERT_EQ(item, item2);

    item2.cost.units = 2222;
    ASSERT_NE(item, item2);
    item2.cost.units = 2;
    ASSERT_EQ(item, item2);

    item2.cost.nano = 3333;
    ASSERT_NE(item, item2);
    item2.cost.nano = 3;
    ASSERT_EQ(item, item2);
}
// NOLINTEND(readability-magic-numbers)
