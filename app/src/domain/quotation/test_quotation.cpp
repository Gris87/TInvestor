#include "src/domain/quotation/quotation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



TEST(Test_Quotation, Test_constructor_and_destructor)
{
    Quotation quotation;

    ASSERT_EQ(quotation.units, 0);
    ASSERT_EQ(quotation.nano, 0);
}

TEST(Test_Quotation, Test_copy_constructor)
{
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 2;

    Quotation quotation2(quotation);

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

    QString content = "{\"nano\":2,\"units\":1}";

    QJsonParseError parseError;
    QJsonDocument   jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    quotation.fromJsonObject(jsonDoc.object());

    ASSERT_EQ(quotation.units, 1);
    ASSERT_EQ(quotation.nano, 2);
}

TEST(Test_Quotation, Test_toJsonObject)
{
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 2;

    QJsonObject   jsonObject = quotation.toJsonObject();
    QJsonDocument jsonDoc(jsonObject);

    QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    QString expectedContent = "{\"nano\":2,\"units\":1}";

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
