#include "src/config/decisions/sell/selldecision4config/selldecision4config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_SellDecision4Config, Test_constructor_and_destructor)
{
    const SellDecision4Config config;
}

TEST(Test_SellDecision4Config, Test_clone_and_deleteRecursively)
{
    SellDecision4Config config;

    config.setEnabled(false);
    config.setLoseYield(3.7f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getLoseYield(), 3.7f);
    // clang-format on

    ISellDecision4Config* config2 = config.clone();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getLoseYield(),   3.7f);
    ASSERT_EQ(config2->isEnabled(),    false);
    ASSERT_EQ(config2->getLoseYield(), 3.7f);
    // clang-format on

    config2->deleteRecursively();
}

TEST(Test_SellDecision4Config, Test_assign)
{
    SellDecision4Config config;
    SellDecision4Config config2;

    config.setEnabled(false);
    config.setLoseYield(3.7f);

    config2.setEnabled(true);
    config2.setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseYield(),  3.7f);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getLoseYield(), 4.2f);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseYield(),  4.2f);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getLoseYield(), 4.2f);
    // clang-format on
}

TEST(Test_SellDecision4Config, Test_makeDefault)
{
    SellDecision4Config config;

    config.setEnabled(false);
    config.setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getLoseYield(), 4.2f);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getLoseYield(), 10.0f);
    // clang-format on
}

TEST(Test_SellDecision4Config, Test_save)
{
    const InSequence seq;

    SellDecision4Config config;

    config.setEnabled(false);
    config.setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getLoseYield(), 4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseYield"), QVariant(4.2f)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision4Config, Test_load)
{
    const InSequence seq;

    SellDecision4Config config;

    config.setEnabled(false);
    config.setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getLoseYield(), 4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseYield"), QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getLoseYield(), 8.9f);
    // clang-format on
}

TEST(Test_SellDecision4Config, Test_fromJsonObject)
{
    SellDecision4Config config;

    const QString content = R"({"enabled":true,"loseYield":"8.90"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getLoseYield(), 8.9f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST(Test_SellDecision4Config, Test_toJsonString)
{
    SellDecision4Config config;

    config.setEnabled(true);
    config.setLoseYield(8.9f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getLoseYield(), 8.9f);
    // clang-format on

    const QString content         = config.toJsonString();
    const QString expectedContent = R"({"enabled":true,"loseYield":"8.90"})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_SellDecision4Config, Test_variantsAsJson)
{
    SellDecision4Config config;

    QStringList variants = config.variantsAsJson();

    ASSERT_EQ(variants.size(), 6);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"loseYield":"5.00"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"loseYield":"7.00"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"loseYield":"10.00"})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"loseYield":"12.00"})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"loseYield":"15.00"})");
}

TEST(Test_SellDecision4Config, Test_setEnabled_and_isEnabled)
{
    SellDecision4Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_SellDecision4Config, Test_setLoseYield_and_getLoseYield)
{
    SellDecision4Config config;

    config.setLoseYield(0.1f);
    ASSERT_EQ(config.getLoseYield(), 0.1f);

    config.setLoseYield(0.5f);
    ASSERT_EQ(config.getLoseYield(), 0.5f);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
