#include "src/config/decisions/buy/buydecision9config/buydecision9config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_BuyDecision9Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new BuyDecision9Config();
    }

    void TearDown() override
    {
        delete config;
    }

    BuyDecision9Config* config;
};



TEST_F(Test_BuyDecision9Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision9Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setRsi(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(), false);
    ASSERT_EQ(config->getRsi(),    123.5f);
    // clang-format on

    IBuyDecision9Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),  false);
    ASSERT_EQ(config->getRsi(),     123.5f);
    ASSERT_EQ(config2->isEnabled(), false);
    ASSERT_EQ(config2->getRsi(),    123.5f);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_BuyDecision9Config, Test_assign)
{
    BuyDecision9Config config2;

    config->setEnabled(false);
    config->setRsi(123.5f);

    config2.setEnabled(true);
    config2.setRsi(321.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),  false);
    ASSERT_EQ(config->getRsi(),     123.5f);
    ASSERT_EQ(config2.isEnabled(),  true);
    ASSERT_EQ(config2.getRsi(),     321.5f);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(), true);
    ASSERT_EQ(config->getRsi(),    321.5f);
    ASSERT_EQ(config2.isEnabled(), true);
    ASSERT_EQ(config2.getRsi(),    321.5f);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setRsi(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(), false);
    ASSERT_EQ(config->getRsi(),    123.5f);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isEnabled(), true);
    ASSERT_EQ(config->getRsi(),    25.0f);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setRsi(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(), false);
    ASSERT_EQ(config->getRsi(),    123.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"), QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Rsi"),     QVariant(123.5f)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_BuyDecision9Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setRsi(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(), false);
    ASSERT_EQ(config->getRsi(),    123.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"), QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Rsi"),     QVariant(123.5f))).WillOnce(Return(QVariant(321.5f)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(), true);
    ASSERT_EQ(config->getRsi(),    321.5f);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"rsi":"321.50"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(), true);
    ASSERT_EQ(config->getRsi(),    321.5f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_BuyDecision9Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setRsi(321.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(), true);
    ASSERT_EQ(config->getRsi(),    321.5f);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"rsi":"321.50"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BuyDecision9Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 5);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"rsi":"20.00"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"rsi":"25.00"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"rsi":"30.00"})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"rsi":"35.00"})");
}

TEST_F(Test_BuyDecision9Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_BuyDecision9Config, Test_setRsi_and_getRsi)
{
    config->setRsi(100);
    ASSERT_EQ(config->getRsi(), 100);

    config->setRsi(20);
    ASSERT_EQ(config->getRsi(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
