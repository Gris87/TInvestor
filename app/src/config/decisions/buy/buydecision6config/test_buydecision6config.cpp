#include "src/config/decisions/buy/buydecision6config/buydecision6config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_BuyDecision6Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new BuyDecision6Config();
    }

    void TearDown() override
    {
        delete config;
    }

    BuyDecision6Config* config;
};



TEST_F(Test_BuyDecision6Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision6Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setRsi(2.5f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsi(),      2.5f);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    IBuyDecision6Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getRsi(),       2.5f);
    ASSERT_EQ(config->getDuration(),  123);
    ASSERT_EQ(config2->isEnabled(),   false);
    ASSERT_EQ(config2->getRsi(),      2.5f);
    ASSERT_EQ(config2->getDuration(), 123);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_BuyDecision6Config, Test_assign)
{
    BuyDecision6Config config2;

    config->setEnabled(false);
    config->setRsi(2.5f);
    config->setDuration(123);

    config2.setEnabled(true);
    config2.setRsi(1.6f);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsi(),      2.5f);
    ASSERT_EQ(config->getDuration(), 123);
    ASSERT_EQ(config2.isEnabled(),   true);
    ASSERT_EQ(config2.getRsi(),      1.6f);
    ASSERT_EQ(config2.getDuration(), 321);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsi(),      1.6f);
    ASSERT_EQ(config->getDuration(), 321);
    ASSERT_EQ(config2.isEnabled(),   true);
    ASSERT_EQ(config2.getRsi(),      1.6f);
    ASSERT_EQ(config2.getDuration(), 321);
    // clang-format on
}

TEST_F(Test_BuyDecision6Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setRsi(2.5f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsi(),      2.5f);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsi(),      25.0f);
    ASSERT_EQ(config->getDuration(), 15);
    // clang-format on
}

TEST_F(Test_BuyDecision6Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setRsi(2.5f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsi(),      2.5f);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),  QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Rsi"),      QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"), QVariant(123)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_BuyDecision6Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setRsi(2.5f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsi(),      2.5f);
    ASSERT_EQ(config->getDuration(), 123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),  QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Rsi"),      QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"), QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsi(),      1.7f);
    ASSERT_EQ(config->getDuration(), 321);
    // clang-format on
}

TEST_F(Test_BuyDecision6Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"rsi":"1.70","duration":321})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsi(),      1.7f);
    ASSERT_EQ(config->getDuration(), 321);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_BuyDecision6Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setRsi(1.7f);
    config->setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsi(),      1.7f);
    ASSERT_EQ(config->getDuration(), 321);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"rsi":"1.70","duration":321})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BuyDecision6Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 13);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"rsi":"20.00","duration":15})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"rsi":"20.00","duration":20})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"rsi":"20.00","duration":30})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"rsi":"25.00","duration":15})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"rsi":"25.00","duration":20})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"rsi":"25.00","duration":30})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"rsi":"30.00","duration":15})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"rsi":"30.00","duration":20})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"rsi":"30.00","duration":30})");
    ASSERT_EQ(variants.at(10), R"({"enabled":true,"rsi":"35.00","duration":15})");
    ASSERT_EQ(variants.at(11), R"({"enabled":true,"rsi":"35.00","duration":20})");
    ASSERT_EQ(variants.at(12), R"({"enabled":true,"rsi":"35.00","duration":30})");
}

TEST_F(Test_BuyDecision6Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_BuyDecision6Config, Test_setRsi_and_getRsi)
{
    config->setRsi(0.1f);
    ASSERT_EQ(config->getRsi(), 0.1f);

    config->setRsi(0.5f);
    ASSERT_EQ(config->getRsi(), 0.5f);
}

TEST_F(Test_BuyDecision6Config, Test_setDuration_and_getDuration)
{
    config->setDuration(100);
    ASSERT_EQ(config->getDuration(), 100);

    config->setDuration(20);
    ASSERT_EQ(config->getDuration(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
