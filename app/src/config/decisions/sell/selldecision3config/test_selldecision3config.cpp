#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_SellDecision3Config, Test_constructor_and_destructor)
{
    const SellDecision3Config config;
}

TEST(Test_SellDecision3Config, Test_assign)
{
    SellDecision3Config config;
    SellDecision3Config config2;

    config.setEnabled(false);
    config.setLoseIncome(3.7f);
    config.setDuration(2);

    config2.setEnabled(true);
    config2.setLoseIncome(4.2f);
    config2.setDuration(6);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getLoseIncome(),  3.7f);
    ASSERT_EQ(config.getDuration(),    2);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getLoseIncome(), 4.2f);
    ASSERT_EQ(config2.getDuration(),   6);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);
    ASSERT_EQ(config.getDuration(),    6);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getLoseIncome(), 4.2f);
    ASSERT_EQ(config2.getDuration(),   6);
    // clang-format on
}

TEST(Test_SellDecision3Config, Test_makeDefault)
{
    SellDecision3Config config;

    config.setEnabled(false);
    config.setLoseIncome(4.2f);
    config.setDuration(2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseIncome(), 4.2f);
    ASSERT_EQ(config.getDuration(),   2);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseIncome(), 5.0f);
    ASSERT_EQ(config.getDuration(),   3);
    // clang-format on
}

TEST(Test_SellDecision3Config, Test_save)
{
    const InSequence seq;

    SellDecision3Config config;

    config.setEnabled(false);
    config.setLoseIncome(4.2f);
    config.setDuration(2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseIncome(), 4.2f);
    ASSERT_EQ(config.getDuration(),   2);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseIncome"), QVariant(4.2f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),   QVariant(2)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision3Config, Test_load)
{
    const InSequence seq;

    SellDecision3Config config;

    config.setEnabled(false);
    config.setLoseIncome(4.2f);
    config.setDuration(2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseIncome(), 4.2f);
    ASSERT_EQ(config.getDuration(),   2);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseIncome"), QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),   QVariant(2))).WillOnce(Return(QVariant(1)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseIncome(), 8.9f);
    ASSERT_EQ(config.getDuration(),   1);
    // clang-format on
}

TEST(Test_SellDecision3Config, Test_fromJsonObject)
{
    SellDecision3Config config;

    const QString content = R"({"enabled":true,"loseIncome":"8.90","duration":1})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseIncome(), 8.9f);
    ASSERT_EQ(config.getDuration(),   1);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST(Test_SellDecision3Config, Test_toJsonString)
{
    SellDecision3Config config;

    config.setEnabled(true);
    config.setLoseIncome(8.9f);
    config.setDuration(1);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseIncome(), 8.9f);
    ASSERT_EQ(config.getDuration(),   1);
    // clang-format on

    const QString content         = config.toJsonString();
    const QString expectedContent = R"({"enabled":true,"loseIncome":"8.90","duration":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_SellDecision3Config, Test_variantsAsJson)
{
    SellDecision3Config config;

    QStringList variants = config.variantsAsJson();

    ASSERT_EQ(variants.size(), 10);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"loseIncome":"3.00","duration":5})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"loseIncome":"3.00","duration":15})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"loseIncome":"3.00","duration":30})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"loseIncome":"4.00","duration":5})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"loseIncome":"4.00","duration":15})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"loseIncome":"4.00","duration":30})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"loseIncome":"5.00","duration":5})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"loseIncome":"5.00","duration":15})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"loseIncome":"5.00","duration":30})");
}

TEST(Test_SellDecision3Config, Test_setEnabled_and_isEnabled)
{
    SellDecision3Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_SellDecision3Config, Test_setLoseIncome_and_getLoseIncome)
{
    SellDecision3Config config;

    config.setLoseIncome(0.1f);
    ASSERT_EQ(config.getLoseIncome(), 0.1f);

    config.setLoseIncome(0.5f);
    ASSERT_EQ(config.getLoseIncome(), 0.5f);
}

TEST(Test_SellDecision3Config, Test_setDuration_and_getDuration)
{
    SellDecision3Config config;

    config.setDuration(1);
    ASSERT_EQ(config.getDuration(), 1);

    config.setDuration(5);
    ASSERT_EQ(config.getDuration(), 5);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
