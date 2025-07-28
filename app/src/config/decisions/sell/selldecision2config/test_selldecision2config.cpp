#include "src/config/decisions/sell/selldecision2config/selldecision2config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_SellDecision2Config, Test_constructor_and_destructor)
{
    const SellDecision2Config config;
}

TEST(Test_SellDecision2Config, Test_assign)
{
    SellDecision2Config config;
    SellDecision2Config config2;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(3.7f);

    config2.setEnabled(true);
    config2.setIncomeAbove(1.6f);
    config2.setLoseIncome(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),       false);
    ASSERT_EQ(config.getIncomeAbove(),  2.5f);
    ASSERT_EQ(config.getLoseIncome(),   3.7f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
    ASSERT_EQ(config2.getLoseIncome(),  4.2f);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),       true);
    ASSERT_EQ(config.getIncomeAbove(),  1.6f);
    ASSERT_EQ(config.getLoseIncome(),   4.2f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
    ASSERT_EQ(config2.getLoseIncome(),  4.2f);
    // clang-format on
}

TEST(Test_SellDecision2Config, Test_makeDefault)
{
    SellDecision2Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.0f);
    ASSERT_EQ(config.getLoseIncome(),  0.1f);
    // clang-format on
}

TEST(Test_SellDecision2Config, Test_save)
{
    const InSequence seq;

    SellDecision2Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),     QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/IncomeAbove"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseIncome"),  QVariant(4.2f)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision2Config, Test_load)
{
    const InSequence seq;

    SellDecision2Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),     QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/IncomeAbove"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseIncome"),  QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.7f);
    ASSERT_EQ(config.getLoseIncome(),  8.9f);
    // clang-format on
}

TEST(Test_SellDecision2Config, Test_fromJsonObject)
{
    SellDecision2Config config;

    const QString content = R"({"enabled":true,"incomeAbove":"1.70","loseIncome":"8.90"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.7f);
    ASSERT_EQ(config.getLoseIncome(),  8.9f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST(Test_SellDecision2Config, Test_toJsonString)
{
    SellDecision2Config config;

    config.setEnabled(true);
    config.setIncomeAbove(1.7f);
    config.setLoseIncome(8.9f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.7f);
    ASSERT_EQ(config.getLoseIncome(),  8.9f);
    // clang-format on

    const QString content         = config.toJsonString();
    const QString expectedContent = R"({"enabled":true,"incomeAbove":"1.70","loseIncome":"8.90"})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_SellDecision2Config, Test_variantsAsJson)
{
    SellDecision2Config config;

    QStringList variants = config.variantsAsJson();

    ASSERT_EQ(variants.size(), 10);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"incomeAbove":"3.00","loseIncome":"0.1"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"incomeAbove":"3.00","loseIncome":"0.3"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"incomeAbove":"3.00","loseIncome":"0.7"})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"incomeAbove":"4.00","loseIncome":"0.1"})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"incomeAbove":"4.00","loseIncome":"0.3"})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"incomeAbove":"4.00","loseIncome":"0.7"})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"incomeAbove":"5.00","loseIncome":"0.1"})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"incomeAbove":"5.00","loseIncome":"0.3"})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"incomeAbove":"5.00","loseIncome":"0.7"})");
}

TEST(Test_SellDecision2Config, Test_setEnabled_and_isEnabled)
{
    SellDecision2Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_SellDecision2Config, Test_setIncomeAbove_and_getIncomeAbove)
{
    SellDecision2Config config;

    config.setIncomeAbove(0.1f);
    ASSERT_EQ(config.getIncomeAbove(), 0.1f);

    config.setIncomeAbove(0.5f);
    ASSERT_EQ(config.getIncomeAbove(), 0.5f);
}

TEST(Test_SellDecision2Config, Test_setLoseIncome_and_getLoseIncome)
{
    SellDecision2Config config;

    config.setLoseIncome(0.1f);
    ASSERT_EQ(config.getLoseIncome(), 0.1f);

    config.setLoseIncome(0.5f);
    ASSERT_EQ(config.getLoseIncome(), 0.5f);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
