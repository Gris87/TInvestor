#include "src/config/decisions/sell/selldecision2config/selldecision2config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_SellDecision2Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new SellDecision2Config();
    }

    void TearDown() override
    {
        delete config;
    }

    SellDecision2Config* config;
};



TEST_F(Test_SellDecision2Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision2Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setYieldAbove(2.5f);
    config->setLoseYield(3.7f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    ASSERT_EQ(config->getLoseYield(),  3.7f);
    // clang-format on

    ISellDecision2Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),      false);
    ASSERT_EQ(config->getYieldAbove(),  2.5f);
    ASSERT_EQ(config->getLoseYield(),   3.7f);
    ASSERT_EQ(config2->isEnabled(),     false);
    ASSERT_EQ(config2->getYieldAbove(), 2.5f);
    ASSERT_EQ(config2->getLoseYield(),  3.7f);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_SellDecision2Config, Test_assign)
{
    SellDecision2Config config2;

    config->setEnabled(false);
    config->setYieldAbove(2.5f);
    config->setLoseYield(3.7f);

    config2.setEnabled(true);
    config2.setYieldAbove(1.6f);
    config2.setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    ASSERT_EQ(config->getLoseYield(),  3.7f);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getYieldAbove(), 1.6f);
    ASSERT_EQ(config2.getLoseYield(),  4.2f);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.6f);
    ASSERT_EQ(config->getLoseYield(),  4.2f);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getYieldAbove(), 1.6f);
    ASSERT_EQ(config2.getLoseYield(),  4.2f);
    // clang-format on
}

TEST_F(Test_SellDecision2Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setYieldAbove(2.5f);
    config->setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    ASSERT_EQ(config->getLoseYield(),  4.2f);
    // clang-format on

    config->makeDefault(30);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 1.0f);
    ASSERT_EQ(config->getLoseYield(),  0.1f);
    // clang-format on

    config->makeDefault(5);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 1.0f);
    ASSERT_EQ(config->getLoseYield(),  0.1f);
    // clang-format on

    config->makeDefault(4);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 1.0f);
    ASSERT_EQ(config->getLoseYield(),  0.1f);
    // clang-format on
}

TEST_F(Test_SellDecision2Config, Test_isDefault)
{
    config->makeDefault(4);
    ASSERT_EQ(config->isDefault(4), true);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 1.0f);
    ASSERT_EQ(config->getLoseYield(),  0.1f);
    // clang-format on

    config->setEnabled(true);
    ASSERT_EQ(config->isDefault(4), false);
    config->setEnabled(false);
    ASSERT_EQ(config->isDefault(4), true);

    config->setYieldAbove(10.0f);
    ASSERT_EQ(config->isDefault(4), false);
    config->setYieldAbove(1.0f);
    ASSERT_EQ(config->isDefault(4), true);

    config->setLoseYield(10.0f);
    ASSERT_EQ(config->isDefault(4), false);
    config->setLoseYield(0.1f);
    ASSERT_EQ(config->isDefault(4), true);
}

TEST_F(Test_SellDecision2Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setYieldAbove(2.5f);
    config->setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    ASSERT_EQ(config->getLoseYield(),  4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/YieldAbove"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseYield"),  QVariant(4.2f)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_SellDecision2Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setYieldAbove(2.5f);
    config->setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    ASSERT_EQ(config->getLoseYield(),  4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/YieldAbove"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseYield"),  QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.7f);
    ASSERT_EQ(config->getLoseYield(),  8.9f);
    // clang-format on
}

TEST_F(Test_SellDecision2Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"yieldAbove":"1.70","loseYield":"8.90"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.7f);
    ASSERT_EQ(config->getLoseYield(),  8.9f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_SellDecision2Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setYieldAbove(1.7f);
    config->setLoseYield(8.9f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.7f);
    ASSERT_EQ(config->getLoseYield(),  8.9f);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"yieldAbove":"1.70","loseYield":"8.90"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_SellDecision2Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 7);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"yieldAbove":"1.00","loseYield":"0.1"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"yieldAbove":"1.00","loseYield":"0.3"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"yieldAbove":"1.50","loseYield":"0.1"})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"yieldAbove":"1.50","loseYield":"0.3"})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"yieldAbove":"2.00","loseYield":"0.1"})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"yieldAbove":"2.00","loseYield":"0.3"})");
}

TEST_F(Test_SellDecision2Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_SellDecision2Config, Test_setYieldAbove_and_getYieldAbove)
{
    config->setYieldAbove(0.1f);
    ASSERT_EQ(config->getYieldAbove(), 0.1f);

    config->setYieldAbove(0.5f);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
}

TEST_F(Test_SellDecision2Config, Test_setLoseYield_and_getLoseYield)
{
    config->setLoseYield(0.1f);
    ASSERT_EQ(config->getLoseYield(), 0.1f);

    config->setLoseYield(0.5f);
    ASSERT_EQ(config->getLoseYield(), 0.5f);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
