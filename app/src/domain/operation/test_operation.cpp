#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    ASSERT_EQ(operation.units, 0);
    ASSERT_EQ(operation.nano, 0);
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.units = 1;
    operation.nano  = 2;

    const Operation operation2(operation);

    ASSERT_EQ(operation2.units, 1);
    ASSERT_EQ(operation2.nano, 2);
}

TEST(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.units = 1;
    operation.nano  = 2;

    operation2 = operation;

    ASSERT_EQ(operation2.units, 1);
    ASSERT_EQ(operation2.nano, 2);
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    ASSERT_EQ(operation.units, 0);
    ASSERT_EQ(operation.nano, 0);

    const QString content = R"({"nano":2,"units":1})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    ASSERT_EQ(operation.units, 1);
    ASSERT_EQ(operation.nano, 2);
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.units = 1;
    operation.nano  = 2;

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"nano":2,"units":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.units = 1;
    operation.nano  = 2;

    operation2.units = 1;
    operation2.nano  = 2;

    ASSERT_EQ(operation, operation2);

    operation2.units = 1000;
    ASSERT_NE(operation, operation2);
    operation2.units = 1;
    ASSERT_EQ(operation, operation2);

    operation2.nano = 2000;
    ASSERT_NE(operation, operation2);
    operation2.nano = 2;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
