#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_BuyDecision2Config, Test_constructor_and_destructor)
{
    const BuyDecision2Config config;
}

TEST(Test_BuyDecision2Config, Test_assign)
{
    BuyDecision2Config config;
    BuyDecision2Config config2;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(2);
    config.setDuration(123);

    config2.setEnabled(true);
    config2.setPriceDiff(1.6f);
    config2.setAmountOfTimes(6);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),         false);
    ASSERT_EQ(config.getPriceDiff(),      2.5f);
    ASSERT_EQ(config.getAmountOfTimes(),  2);
    ASSERT_EQ(config.getDuration(),       123);
    ASSERT_EQ(config2.isEnabled(),        true);
    ASSERT_EQ(config2.getPriceDiff(),     1.6f);
    ASSERT_EQ(config2.getAmountOfTimes(), 6);
    ASSERT_EQ(config2.getDuration(),      321);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),         true);
    ASSERT_EQ(config.getPriceDiff(),      1.6f);
    ASSERT_EQ(config.getAmountOfTimes(),  6);
    ASSERT_EQ(config.getDuration(),       321);
    ASSERT_EQ(config2.isEnabled(),        true);
    ASSERT_EQ(config2.getPriceDiff(),     1.6f);
    ASSERT_EQ(config2.getAmountOfTimes(), 6);
    ASSERT_EQ(config2.getDuration(),      321);
    // clang-format on
}

TEST(Test_BuyDecision2Config, Test_makeDefault)
{
    BuyDecision2Config config;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(5);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        false);
    ASSERT_EQ(config.getPriceDiff(),     2.5f);
    ASSERT_EQ(config.getAmountOfTimes(), 5);
    ASSERT_EQ(config.getDuration(),      123);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        true);
    ASSERT_EQ(config.getPriceDiff(),     1.0f);
    ASSERT_EQ(config.getAmountOfTimes(), 2);
    ASSERT_EQ(config.getDuration(),      15);
    // clang-format on
}

TEST(Test_BuyDecision2Config, Test_save)
{
    const InSequence seq;

    BuyDecision2Config config;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(2);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        false);
    ASSERT_EQ(config.getPriceDiff(),     2.5f);
    ASSERT_EQ(config.getAmountOfTimes(), 2);
    ASSERT_EQ(config.getDuration(),      123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceDiff"),     QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/AmountOfTimes"), QVariant(2)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),      QVariant(123)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_BuyDecision2Config, Test_load)
{
    const InSequence seq;

    BuyDecision2Config config;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(2);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        false);
    ASSERT_EQ(config.getPriceDiff(),     2.5f);
    ASSERT_EQ(config.getAmountOfTimes(), 2);
    ASSERT_EQ(config.getDuration(),      123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceDiff"),     QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/AmountOfTimes"), QVariant(2))).WillOnce(Return(QVariant(7)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),      QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        true);
    ASSERT_EQ(config.getPriceDiff(),     1.7f);
    ASSERT_EQ(config.getAmountOfTimes(), 7);
    ASSERT_EQ(config.getDuration(),      321);
    // clang-format on
}

TEST(Test_BuyDecision2Config, Test_fromJsonObject)
{
    BuyDecision2Config config;

    const QString content = R"({"enabled":true,"priceDiff":"1.70","amountOfTimes":7,"duration":321})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        true);
    ASSERT_EQ(config.getPriceDiff(),     1.7f);
    ASSERT_EQ(config.getAmountOfTimes(), 7);
    ASSERT_EQ(config.getDuration(),      321);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST(Test_BuyDecision2Config, Test_toJsonString)
{
    BuyDecision2Config config;

    config.setEnabled(true);
    config.setPriceDiff(1.7f);
    config.setAmountOfTimes(7);
    config.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        true);
    ASSERT_EQ(config.getPriceDiff(),     1.7f);
    ASSERT_EQ(config.getAmountOfTimes(), 7);
    ASSERT_EQ(config.getDuration(),      321);
    // clang-format on

    const QString content         = config.toJsonString();
    const QString expectedContent = R"({"enabled":true,"priceDiff":"1.70","amountOfTimes":7,"duration":321})";

    qInfo() << content;

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_BuyDecision2Config, Test_variantsAsJson)
{
    BuyDecision2Config config;

    QStringList variants = config.variantsAsJson();

    ASSERT_EQ(variants.size(), 28);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":1,"duration":5})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":1,"duration":15})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":1,"duration":30})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":2,"duration":5})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":2,"duration":15})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":2,"duration":30})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":3,"duration":5})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":3,"duration":15})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"priceDiff":"3.00","amountOfTimes":3,"duration":30})");
    ASSERT_EQ(variants.at(10), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":1,"duration":5})");
    ASSERT_EQ(variants.at(11), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":1,"duration":15})");
    ASSERT_EQ(variants.at(12), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":1,"duration":30})");
    ASSERT_EQ(variants.at(13), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":2,"duration":5})");
    ASSERT_EQ(variants.at(14), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":2,"duration":15})");
    ASSERT_EQ(variants.at(15), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":2,"duration":30})");
    ASSERT_EQ(variants.at(16), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":3,"duration":5})");
    ASSERT_EQ(variants.at(17), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":3,"duration":15})");
    ASSERT_EQ(variants.at(18), R"({"enabled":true,"priceDiff":"4.00","amountOfTimes":3,"duration":30})");
    ASSERT_EQ(variants.at(19), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":1,"duration":5})");
    ASSERT_EQ(variants.at(20), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":1,"duration":15})");
    ASSERT_EQ(variants.at(21), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":1,"duration":30})");
    ASSERT_EQ(variants.at(22), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":2,"duration":5})");
    ASSERT_EQ(variants.at(23), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":2,"duration":15})");
    ASSERT_EQ(variants.at(24), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":2,"duration":30})");
    ASSERT_EQ(variants.at(25), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":3,"duration":5})");
    ASSERT_EQ(variants.at(26), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":3,"duration":15})");
    ASSERT_EQ(variants.at(27), R"({"enabled":true,"priceDiff":"5.00","amountOfTimes":3,"duration":30})");
}

TEST(Test_BuyDecision2Config, Test_setEnabled_and_isEnabled)
{
    BuyDecision2Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_BuyDecision2Config, Test_setPriceDiff_and_getPriceDiff)
{
    BuyDecision2Config config;

    config.setPriceDiff(0.1f);
    ASSERT_EQ(config.getPriceDiff(), 0.1f);

    config.setPriceDiff(0.5f);
    ASSERT_EQ(config.getPriceDiff(), 0.5f);
}

TEST(Test_BuyDecision2Config, Test_setAmountOfTimes_and_getAmountOfTimes)
{
    BuyDecision2Config config;

    config.setAmountOfTimes(1);
    ASSERT_EQ(config.getAmountOfTimes(), 1);

    config.setAmountOfTimes(5);
    ASSERT_EQ(config.getAmountOfTimes(), 5);
}

TEST(Test_BuyDecision2Config, Test_setDuration_and_getDuration)
{
    BuyDecision2Config config;

    config.setDuration(100);
    ASSERT_EQ(config.getDuration(), 100);

    config.setDuration(20);
    ASSERT_EQ(config.getDuration(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
