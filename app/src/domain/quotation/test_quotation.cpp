#include "src/domain/quotation/quotation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Quotation, Test_constructor_and_destructor)
{
    const Quotation quotation;

    ASSERT_EQ(quotation.units, 0);
    ASSERT_EQ(quotation.nano, 0);
}

TEST(Test_Quotation, Test_copy_constructor)
{
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 2;

    const Quotation quotation2(quotation);

    ASSERT_EQ(quotation2.units, 1);
    ASSERT_EQ(quotation2.nano, 2);
}

TEST(Test_Quotation, Test_assign)
{
    Quotation quotation;
    Quotation quotation2;

    quotation.units = 1;
    quotation.nano  = 2;

    quotation2 = quotation;

    ASSERT_EQ(quotation2.units, 1);
    ASSERT_EQ(quotation2.nano, 2);
}

TEST(Test_Quotation, Test_fromJsonObject)
{
    Quotation quotation;

    ASSERT_EQ(quotation.units, 0);
    ASSERT_EQ(quotation.nano, 0);

    const QString content = R"({"nano":2,"units":1})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    quotation.fromJsonObject(doc.get_object());

    ASSERT_EQ(quotation.units, 1);
    ASSERT_EQ(quotation.nano, 2);
}

TEST(Test_Quotation, Test_toJsonObject)
{
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 2;

    const QJsonObject   jsonObject = quotation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"nano":2,"units":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Quotation, Test_equals)
{
    Quotation quotation;
    Quotation quotation2;

    quotation.units = 1;
    quotation.nano  = 2;

    quotation2.units = 1;
    quotation2.nano  = 2;

    ASSERT_EQ(quotation, quotation2);

    quotation2.units = 1000;
    ASSERT_NE(quotation, quotation2);
    quotation2.units = 1;
    ASSERT_EQ(quotation, quotation2);

    quotation2.nano = 2000;
    ASSERT_NE(quotation, quotation2);
    quotation2.nano = 2;
    ASSERT_EQ(quotation, quotation2);
}
// NOLINTEND(readability-magic-numbers)
