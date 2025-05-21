#include "src/domain/instrument/instrument.h"

#include <QJsonDocument>
#include <gtest/gtest.h>



TEST(Test_Instrument, Test_constructor_and_destructor)
{
    const Instrument instrument;

    // clang-format off
    ASSERT_EQ(instrument.ticker,         "");
    ASSERT_EQ(instrument.name,           "");
    ASSERT_EQ(instrument.pricePrecision, 0);
    // clang-format on
}

TEST(Test_Instrument, Test_copy_constructor)
{
    Instrument instrument;

    instrument.ticker         = "a";
    instrument.name           = "b";
    instrument.pricePrecision = 1;

    const Instrument instrument2(instrument);

    // clang-format off
    ASSERT_EQ(instrument2.ticker,         "a");
    ASSERT_EQ(instrument2.name,           "b");
    ASSERT_EQ(instrument2.pricePrecision, 1);
    // clang-format on
}

TEST(Test_Instrument, Test_assign)
{
    Instrument instrument;
    Instrument instrument2;

    instrument.ticker         = "a";
    instrument.name           = "b";
    instrument.pricePrecision = 1;

    instrument2 = instrument;

    // clang-format off
    ASSERT_EQ(instrument2.ticker,         "a");
    ASSERT_EQ(instrument2.name,           "b");
    ASSERT_EQ(instrument2.pricePrecision, 1);
    // clang-format on
}

TEST(Test_Instrument, Test_fromJsonObject)
{
    Instrument instrument;

    // clang-format off
    ASSERT_EQ(instrument.ticker,         "");
    ASSERT_EQ(instrument.name,           "");
    ASSERT_EQ(instrument.pricePrecision, 0);
    // clang-format on

    const QString content = R"({"name":"b","pricePrecision":1,"ticker":"a"})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    instrument.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(instrument.ticker,         "a");
    ASSERT_EQ(instrument.name,           "b");
    ASSERT_EQ(instrument.pricePrecision, 1);
    // clang-format on
}

TEST(Test_Instrument, Test_toJsonObject)
{
    Instrument instrument;

    instrument.ticker         = "a";
    instrument.name           = "b";
    instrument.pricePrecision = 1;

    const QJsonObject   jsonObject = instrument.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"name":"b","pricePrecision":1,"ticker":"a"})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Instrument, Test_equals)
{
    Instrument instrument;
    Instrument instrument2;

    instrument.ticker         = "a";
    instrument.name           = "b";
    instrument.pricePrecision = 1;

    instrument2.ticker         = "a";
    instrument2.name           = "b";
    instrument2.pricePrecision = 1;

    ASSERT_EQ(instrument, instrument2);

    instrument2.ticker = "aaaa";
    ASSERT_NE(instrument, instrument2);
    instrument2.ticker = "a";
    ASSERT_EQ(instrument, instrument2);

    instrument2.name = "bbbb";
    ASSERT_NE(instrument, instrument2);
    instrument2.name = "b";
    ASSERT_EQ(instrument, instrument2);

    instrument2.pricePrecision = -1;
    ASSERT_NE(instrument, instrument2);
    instrument2.pricePrecision = 1;
    ASSERT_EQ(instrument, instrument2);
}
