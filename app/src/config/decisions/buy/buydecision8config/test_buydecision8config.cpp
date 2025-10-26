#include "src/config/decisions/buy/buydecision8config/buydecision8config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_BuyDecision8Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new BuyDecision8Config();
    }

    void TearDown() override
    {
        delete config;
    }

    BuyDecision8Config* config;
};



TEST_F(Test_BuyDecision8Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision8Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    IBuyDecision8Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getDuration(),  123);
    ASSERT_EQ(config2->isEnabled(),   false);
    ASSERT_EQ(config2->getDuration(), 123);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_BuyDecision8Config, Test_assign)
{
    BuyDecision8Config config2;

    config->setEnabled(false);
    config->setDuration(123);

    config2.setEnabled(true);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getDuration(), 123);
    ASSERT_EQ(config2.isEnabled(),   true);
    ASSERT_EQ(config2.getDuration(), 321);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 321);
    ASSERT_EQ(config2.isEnabled(),   true);
    ASSERT_EQ(config2.getDuration(), 321);
    // clang-format on
}

TEST_F(Test_BuyDecision8Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    config->makeDefault(30);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 30);
    // clang-format on

    config->makeDefault(5);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 30);
    // clang-format on

    config->makeDefault(4);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 30);
    // clang-format on
}

TEST_F(Test_BuyDecision8Config, Test_isDefault)
{
    config->makeDefault(4);
    ASSERT_EQ(config->isDefault(4), true);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 30);
    // clang-format on

    config->setEnabled(false);
    ASSERT_EQ(config->isDefault(4), false);
    config->setEnabled(true);
    ASSERT_EQ(config->isDefault(4), true);

    config->setDuration(123);
    ASSERT_EQ(config->isDefault(4), false);
    config->setDuration(30);
    ASSERT_EQ(config->isDefault(4), true);
}

TEST_F(Test_BuyDecision8Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),  QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"), QVariant(123)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_BuyDecision8Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),  QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"), QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 321);
    // clang-format on
}

TEST_F(Test_BuyDecision8Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"duration":321})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 321);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_BuyDecision8Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getDuration(), 321);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"duration":321})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BuyDecision8Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 6);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"duration":15})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"duration":30})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"duration":60})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"duration":120})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"duration":180})");
}

TEST_F(Test_BuyDecision8Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_BuyDecision8Config, Test_setDuration_and_getDuration)
{
    config->setDuration(100);
    ASSERT_EQ(config->getDuration(), 100);

    config->setDuration(20);
    ASSERT_EQ(config->getDuration(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
