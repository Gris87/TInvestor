#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,    0);
    ASSERT_EQ(operation.instrumentId, "");
    // clang-format on
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp    = 1;
    operation.instrumentId = "a";

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,    1);
    ASSERT_EQ(operation2.instrumentId, "a");
    // clang-format on
}

TEST(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.timestamp    = 1;
    operation.instrumentId = "a";

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,    1);
    ASSERT_EQ(operation2.instrumentId, "a");
    // clang-format on
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,    0);
    ASSERT_EQ(operation.instrumentId, "");
    // clang-format on

    const QString content = R"({"instrumentId":"a","timestamp":1})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,    1);
    ASSERT_EQ(operation.instrumentId, "a");
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp    = 1;
    operation.instrumentId = "a";

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"instrumentId":"a","timestamp":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.timestamp    = 1;
    operation.instrumentId = "a";

    operation2.timestamp    = 1;
    operation2.instrumentId = "a";

    ASSERT_EQ(operation, operation2);

    operation2.timestamp = 1000;
    ASSERT_NE(operation, operation2);
    operation2.timestamp = 1;
    ASSERT_EQ(operation, operation2);

    operation2.instrumentId = "aaaa";
    ASSERT_NE(operation, operation2);
    operation2.instrumentId = "a";
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
