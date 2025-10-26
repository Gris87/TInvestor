#include "src/config/decisions/sell/selldecision1config/selldecision1config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_SellDecision1Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new SellDecision1Config();
    }

    void TearDown() override
    {
        delete config;
    }

    SellDecision1Config* config;
};



TEST_F(Test_SellDecision1Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision1Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setYieldAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    // clang-format on

    ISellDecision1Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),      false);
    ASSERT_EQ(config->getYieldAbove(),  2.5f);
    ASSERT_EQ(config2->isEnabled(),     false);
    ASSERT_EQ(config2->getYieldAbove(), 2.5f);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_SellDecision1Config, Test_assign)
{
    SellDecision1Config config2;

    config->setEnabled(false);
    config->setYieldAbove(2.5f);

    config2.setEnabled(true);
    config2.setYieldAbove(1.6f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getYieldAbove(), 1.6f);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.6f);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getYieldAbove(), 1.6f);
    // clang-format on
}

TEST_F(Test_SellDecision1Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setYieldAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    // clang-format on

    config->makeDefault(30);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
    // clang-format on

    config->makeDefault(5);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
    // clang-format on

    config->makeDefault(4);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
    // clang-format on
}

TEST_F(Test_SellDecision1Config, Test_isDefault)
{
    config->makeDefault(4);
    ASSERT_EQ(config->isDefault(4), true);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
    // clang-format on

    config->setEnabled(false);
    ASSERT_EQ(config->isDefault(4), false);
    config->setEnabled(true);
    ASSERT_EQ(config->isDefault(4), true);

    config->setYieldAbove(24.5f);
    ASSERT_EQ(config->isDefault(4), false);
    config->setYieldAbove(0.5f);
    ASSERT_EQ(config->isDefault(4), true);
}

TEST_F(Test_SellDecision1Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setYieldAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/YieldAbove"), QVariant(2.5f)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_SellDecision1Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setYieldAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getYieldAbove(), 2.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/YieldAbove"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.7f);
    // clang-format on
}

TEST_F(Test_SellDecision1Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"yieldAbove":"1.70"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.7f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_SellDecision1Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setYieldAbove(1.7f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getYieldAbove(), 1.7f);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"yieldAbove":"1.70"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_SellDecision1Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 4);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"yieldAbove":"1.00"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"yieldAbove":"1.50"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"yieldAbove":"2.00"})");
}

TEST_F(Test_SellDecision1Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_SellDecision1Config, Test_setYieldAbove_and_getYieldAbove)
{
    config->setYieldAbove(0.1f);
    ASSERT_EQ(config->getYieldAbove(), 0.1f);

    config->setYieldAbove(0.5f);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
