#include "src/config/decisions/sell/selldecision5config/selldecision5config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_SellDecision5Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new SellDecision5Config();
    }

    void TearDown() override
    {
        delete config;
    }

    SellDecision5Config* config;
};



TEST_F(Test_SellDecision5Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision5Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setDuration(2);
    config->setYieldAbove(3.7f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getDuration(),   2);
    ASSERT_EQ(config->getYieldAbove(), 3.7f);
    // clang-format on

    ISellDecision5Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),      false);
    ASSERT_EQ(config->getDuration(),    2);
    ASSERT_EQ(config->getYieldAbove(),  3.7f);
    ASSERT_EQ(config2->isEnabled(),     false);
    ASSERT_EQ(config2->getDuration(),   2);
    ASSERT_EQ(config2->getYieldAbove(), 3.7f);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_SellDecision5Config, Test_assign)
{
    SellDecision5Config config2;

    config->setEnabled(false);
    config->setDuration(2);
    config->setYieldAbove(3.7f);

    config2.setEnabled(true);
    config2.setDuration(6);
    config2.setYieldAbove(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getDuration(),   2);
    ASSERT_EQ(config->getYieldAbove(), 3.7f);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getDuration(),   6);
    ASSERT_EQ(config2.getYieldAbove(), 4.2f);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getDuration(),   6);
    ASSERT_EQ(config->getYieldAbove(), 4.2f);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getDuration(),   6);
    ASSERT_EQ(config2.getYieldAbove(), 4.2f);
    // clang-format on
}

TEST_F(Test_SellDecision5Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setDuration(2);
    config->setYieldAbove(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getDuration(),   2);
    ASSERT_EQ(config->getYieldAbove(), 4.2f);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getDuration(),   15);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
    // clang-format on
}

TEST_F(Test_SellDecision5Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setDuration(2);
    config->setYieldAbove(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getDuration(),   2);
    ASSERT_EQ(config->getYieldAbove(), 4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),   QVariant(2)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/YieldAbove"), QVariant(4.2f)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_SellDecision5Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setDuration(2);
    config->setYieldAbove(4.2f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getDuration(),   2);
    ASSERT_EQ(config->getYieldAbove(), 4.2f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),   QVariant(2))).WillOnce(Return(QVariant(1)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/YieldAbove"), QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getDuration(),   1);
    ASSERT_EQ(config->getYieldAbove(), 8.9f);
    // clang-format on
}

TEST_F(Test_SellDecision5Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"duration":1,"yieldAbove":"8.90"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getDuration(),   1);
    ASSERT_EQ(config->getYieldAbove(), 8.9f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_SellDecision5Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setDuration(1);
    config->setYieldAbove(8.9f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     true);
    ASSERT_EQ(config->getDuration(),   1);
    ASSERT_EQ(config->getYieldAbove(), 8.9f);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"duration":1,"yieldAbove":"8.90"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_SellDecision5Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 10);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"duration":15,"yieldAbove":"0.50"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"duration":15,"yieldAbove":"1.00"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"duration":15,"yieldAbove":"1.50"})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"duration":30,"yieldAbove":"0.50"})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"duration":30,"yieldAbove":"1.00"})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"duration":30,"yieldAbove":"1.50"})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"duration":60,"yieldAbove":"0.50"})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"duration":60,"yieldAbove":"1.00"})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"duration":60,"yieldAbove":"1.50"})");
}

TEST_F(Test_SellDecision5Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_SellDecision5Config, Test_setDuration_and_getDuration)
{
    config->setDuration(1);
    ASSERT_EQ(config->getDuration(), 1);

    config->setDuration(5);
    ASSERT_EQ(config->getDuration(), 5);
}

TEST_F(Test_SellDecision5Config, Test_setYieldAbove_and_getYieldAbove)
{
    config->setYieldAbove(0.1f);
    ASSERT_EQ(config->getYieldAbove(), 0.1f);

    config->setYieldAbove(0.5f);
    ASSERT_EQ(config->getYieldAbove(), 0.5f);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
