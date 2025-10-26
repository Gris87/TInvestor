#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_SellDecision3Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new SellDecision3Config();
    }

    void TearDown() override
    {
        delete config;
    }

    SellDecision3Config* config;
};



TEST_F(Test_SellDecision3Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision3Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setLoseYield(3.7f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getLoseYield(), 3.7f);
    // clang-format on

    ISellDecision3Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getLoseYield(),  3.7f);
    ASSERT_EQ(config2->isEnabled(),    false);
    ASSERT_EQ(config2->getLoseYield(), 3.7f);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_SellDecision3Config, Test_assign)
{
    SellDecision3Config config2;

    config->setEnabled(false);
    config->setLoseYield(3.7f);

    config2.setEnabled(true);
    config2.setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getLoseYield(), 3.7f);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getLoseYield(), 4.2f);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getLoseYield(), 4.2f);
    // clang-format on
}

TEST_F(Test_SellDecision3Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    // clang-format on

    config->makeDefault(30);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 3.0f);
    // clang-format on

    config->makeDefault(5);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 3.0f);
    // clang-format on

    config->makeDefault(4);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 3.0f);
    // clang-format on
}

TEST_F(Test_SellDecision3Config, Test_isDefault)
{
    config->makeDefault(4);
    ASSERT_EQ(config->isDefault(4), true);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 3.0f);
    // clang-format on

    config->setEnabled(false);
    ASSERT_EQ(config->isDefault(4), false);
    config->setEnabled(true);
    ASSERT_EQ(config->isDefault(4), true);

    config->setLoseYield(24.5f);
    ASSERT_EQ(config->isDefault(4), false);
    config->setLoseYield(3.0f);
    ASSERT_EQ(config->isDefault(4), true);
}

TEST_F(Test_SellDecision3Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseYield"), QVariant(4.2f)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_SellDecision3Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setLoseYield(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseYield"), QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 8.9f);
    // clang-format on
}

TEST_F(Test_SellDecision3Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"loseYield":"8.90"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 8.9f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_SellDecision3Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setLoseYield(8.9f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getLoseYield(), 8.9f);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"loseYield":"8.90"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_SellDecision3Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 4);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"loseYield":"3.00"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"loseYield":"5.00"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"loseYield":"7.00"})");
}

TEST_F(Test_SellDecision3Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_SellDecision3Config, Test_setLoseYield_and_getLoseYield)
{
    config->setLoseYield(0.1f);
    ASSERT_EQ(config->getLoseYield(), 0.1f);

    config->setLoseYield(0.5f);
    ASSERT_EQ(config->getLoseYield(), 0.5f);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
