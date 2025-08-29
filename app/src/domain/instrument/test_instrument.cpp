#include "src/domain/instrument/instrument.h"

#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



class Test_Instrument : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_Instrument, Test_constructor_and_destructor)
{
    const Instrument instrument;

    // clang-format off
    ASSERT_EQ(instrument.ticker,                  "");
    ASSERT_EQ(instrument.name,                    "");
    ASSERT_EQ(instrument.lot,                     0);
    ASSERT_EQ(instrument.pricePrecision,          0);
    ASSERT_EQ(instrument.minPriceIncrement.units, 0);
    ASSERT_EQ(instrument.minPriceIncrement.nano,  0);
    // clang-format on
}

TEST_F(Test_Instrument, Test_copy_constructor)
{
    Instrument instrument;

    instrument.ticker                  = "a";
    instrument.name                    = "b";
    instrument.lot                     = 1;
    instrument.pricePrecision          = 2;
    instrument.minPriceIncrement.units = 3;
    instrument.minPriceIncrement.nano  = 4;

    const Instrument instrument2(instrument);

    // clang-format off
    ASSERT_EQ(instrument2.ticker,                  "a");
    ASSERT_EQ(instrument2.name,                    "b");
    ASSERT_EQ(instrument2.lot,                     1);
    ASSERT_EQ(instrument2.pricePrecision,          2);
    ASSERT_EQ(instrument2.minPriceIncrement.units, 3);
    ASSERT_EQ(instrument2.minPriceIncrement.nano,  4);
    // clang-format on
}

TEST_F(Test_Instrument, Test_assign)
{
    Instrument instrument;
    Instrument instrument2;

    instrument.ticker                  = "a";
    instrument.name                    = "b";
    instrument.lot                     = 1;
    instrument.pricePrecision          = 2;
    instrument.minPriceIncrement.units = 3;
    instrument.minPriceIncrement.nano  = 4;

    instrument2 = instrument;

    // clang-format off
    ASSERT_EQ(instrument2.ticker,                  "a");
    ASSERT_EQ(instrument2.name,                    "b");
    ASSERT_EQ(instrument2.lot,                     1);
    ASSERT_EQ(instrument2.pricePrecision,          2);
    ASSERT_EQ(instrument2.minPriceIncrement.units, 3);
    ASSERT_EQ(instrument2.minPriceIncrement.nano,  4);
    // clang-format on
}

TEST_F(Test_Instrument, Test_resetIfNotFound)
{
    Instrument instrument;

    // clang-format off
    ASSERT_EQ(instrument.ticker,                  "");
    ASSERT_EQ(instrument.name,                    "");
    ASSERT_EQ(instrument.lot,                     0);
    ASSERT_EQ(instrument.pricePrecision,          0);
    ASSERT_EQ(instrument.minPriceIncrement.units, 0);
    ASSERT_EQ(instrument.minPriceIncrement.nano,  0);
    // clang-format on

    instrument.resetIfNotFound("aaaaa");

    // clang-format off
    ASSERT_EQ(instrument.ticker,                  "aaaaa");
    ASSERT_EQ(instrument.name,                    "?????");
    ASSERT_EQ(instrument.lot,                     1);
    ASSERT_EQ(instrument.pricePrecision,          2);
    ASSERT_EQ(instrument.minPriceIncrement.units, 0);
    ASSERT_EQ(instrument.minPriceIncrement.nano,  0);
    // clang-format on
}

TEST_F(Test_Instrument, Test_fromJsonObject)
{
    Instrument instrument;

    // clang-format off
    ASSERT_EQ(instrument.ticker,                  "");
    ASSERT_EQ(instrument.name,                    "");
    ASSERT_EQ(instrument.lot,                     0);
    ASSERT_EQ(instrument.pricePrecision,          0);
    ASSERT_EQ(instrument.minPriceIncrement.units, 0);
    ASSERT_EQ(instrument.minPriceIncrement.nano,  0);
    // clang-format on

    const QString content = R"({"lot":1,"minPriceIncrement":{"nano":4,"units":3},"name":"b","pricePrecision":2,"ticker":"a"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    instrument.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(instrument.ticker,                  "a");
    ASSERT_EQ(instrument.name,                    "b");
    ASSERT_EQ(instrument.lot,                     1);
    ASSERT_EQ(instrument.pricePrecision,          2);
    ASSERT_EQ(instrument.minPriceIncrement.units, 3);
    ASSERT_EQ(instrument.minPriceIncrement.nano,  4);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    instrument.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_Instrument, Test_toJsonObject)
{
    Instrument instrument;

    instrument.ticker                  = "a";
    instrument.name                    = "b";
    instrument.lot                     = 1;
    instrument.pricePrecision          = 2;
    instrument.minPriceIncrement.units = 3;
    instrument.minPriceIncrement.nano  = 4;

    const QJsonObject   jsonObject = instrument.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"lot":1,"minPriceIncrement":{"nano":4,"units":3},"name":"b","pricePrecision":2,"ticker":"a"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_Instrument, Test_equals)
{
    Instrument instrument;
    Instrument instrument2;

    instrument.ticker                  = "a";
    instrument.name                    = "b";
    instrument.lot                     = 1;
    instrument.pricePrecision          = 2;
    instrument.minPriceIncrement.units = 3;
    instrument.minPriceIncrement.nano  = 4;

    instrument2.ticker                  = "a";
    instrument2.name                    = "b";
    instrument2.lot                     = 1;
    instrument2.pricePrecision          = 2;
    instrument2.minPriceIncrement.units = 3;
    instrument2.minPriceIncrement.nano  = 4;

    ASSERT_EQ(instrument, instrument2);

    instrument2.ticker = "aaaa";
    ASSERT_NE(instrument, instrument2);
    instrument2.ticker = "a";
    ASSERT_EQ(instrument, instrument2);

    instrument2.name = "bbbb";
    ASSERT_NE(instrument, instrument2);
    instrument2.name = "b";
    ASSERT_EQ(instrument, instrument2);

    instrument2.lot = -1;
    ASSERT_NE(instrument, instrument2);
    instrument2.lot = 1;
    ASSERT_EQ(instrument, instrument2);

    instrument2.pricePrecision = -2;
    ASSERT_NE(instrument, instrument2);
    instrument2.pricePrecision = 2;
    ASSERT_EQ(instrument, instrument2);

    instrument2.minPriceIncrement.units = -3;
    ASSERT_NE(instrument, instrument2);
    instrument2.minPriceIncrement.units = 3;
    ASSERT_EQ(instrument, instrument2);

    instrument2.minPriceIncrement.nano = -4;
    ASSERT_NE(instrument, instrument2);
    instrument2.minPriceIncrement.nano = 4;
    ASSERT_EQ(instrument, instrument2);
}
