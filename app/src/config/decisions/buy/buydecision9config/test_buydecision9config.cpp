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
    config->setRsiMonth(2.5f);
    config->setRsiWeek(3.5f);
    config->setRsiDay(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsiMonth(), 2.5f);
    ASSERT_EQ(config->getRsiWeek(),  3.5f);
    ASSERT_EQ(config->getRsiDay(),   123.5f);
    // clang-format on

    IBuyDecision9Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getRsiMonth(),  2.5f);
    ASSERT_EQ(config->getRsiWeek(),   3.5f);
    ASSERT_EQ(config->getRsiDay(),    123.5f);
    ASSERT_EQ(config2->isEnabled(),   false);
    ASSERT_EQ(config2->getRsiMonth(), 2.5f);
    ASSERT_EQ(config2->getRsiWeek(),  3.5f);
    ASSERT_EQ(config2->getRsiDay(),   123.5f);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_BuyDecision9Config, Test_assign)
{
    BuyDecision9Config config2;

    config->setEnabled(false);
    config->setRsiMonth(2.5f);
    config->setRsiWeek(3.5f);
    config->setRsiDay(123.5f);

    config2.setEnabled(true);
    config2.setRsiMonth(1.6f);
    config2.setRsiWeek(4.5f);
    config2.setRsiDay(321.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsiMonth(), 2.5f);
    ASSERT_EQ(config->getRsiWeek(),  3.5f);
    ASSERT_EQ(config->getRsiDay(),   123.5f);
    ASSERT_EQ(config2.isEnabled(),   true);
    ASSERT_EQ(config2.getRsiMonth(), 1.6f);
    ASSERT_EQ(config2.getRsiWeek(),  4.5f);
    ASSERT_EQ(config2.getRsiDay(),   321.5f);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsiMonth(), 1.6f);
    ASSERT_EQ(config->getRsiWeek(),  4.5f);
    ASSERT_EQ(config->getRsiDay(),   321.5f);
    ASSERT_EQ(config2.isEnabled(),   true);
    ASSERT_EQ(config2.getRsiMonth(), 1.6f);
    ASSERT_EQ(config2.getRsiWeek(),  4.5f);
    ASSERT_EQ(config2.getRsiDay(),   321.5f);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setRsiMonth(2.5f);
    config->setRsiWeek(4.5f);
    config->setRsiDay(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsiMonth(), 2.5f);
    ASSERT_EQ(config->getRsiWeek(),  4.5f);
    ASSERT_EQ(config->getRsiDay(),   123.5f);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsiMonth(), 70.0f);
    ASSERT_EQ(config->getRsiWeek(),  60.0f);
    ASSERT_EQ(config->getRsiDay(),   60.0f);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setRsiMonth(2.5f);
    config->setRsiWeek(4.5f);
    config->setRsiDay(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsiMonth(), 2.5f);
    ASSERT_EQ(config->getRsiWeek(),  4.5f);
    ASSERT_EQ(config->getRsiDay(),   123.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),  QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/RsiMonth"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/RsiWeek"),  QVariant(4.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/RsiDay"),   QVariant(123.5f)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_BuyDecision9Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setRsiMonth(2.5f);
    config->setRsiWeek(4.5f);
    config->setRsiDay(123.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   false);
    ASSERT_EQ(config->getRsiMonth(), 2.5f);
    ASSERT_EQ(config->getRsiWeek(),  4.5f);
    ASSERT_EQ(config->getRsiDay(),   123.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),  QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/RsiMonth"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/RsiWeek"),  QVariant(4.5f))).WillOnce(Return(QVariant(8.5f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/RsiDay"),   QVariant(123.5f))).WillOnce(Return(QVariant(321.5f)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsiMonth(), 1.7f);
    ASSERT_EQ(config->getRsiWeek(),  8.5f);
    ASSERT_EQ(config->getRsiDay(),   321.5f);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"rsiMonth":"1.70","rsiWeek":"8.50","rsiDay":"321.50"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsiMonth(), 1.7f);
    ASSERT_EQ(config->getRsiWeek(),  8.5f);
    ASSERT_EQ(config->getRsiDay(),   321.5f);
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
    config->setRsiMonth(1.7f);
    config->setRsiWeek(8.5f);
    config->setRsiDay(321.5f);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),   true);
    ASSERT_EQ(config->getRsiMonth(), 1.7f);
    ASSERT_EQ(config->getRsiWeek(),  8.5f);
    ASSERT_EQ(config->getRsiDay(),   321.5f);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"rsiMonth":"1.70","rsiWeek":"8.50","rsiDay":"321.50"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BuyDecision9Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 28);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"60.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"60.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"60.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"70.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"70.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"70.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"80.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"80.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"rsiMonth":"60.00","rsiWeek":"80.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(10), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"60.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(11), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"60.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(12), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"60.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(13), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"70.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(14), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"70.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(15), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"70.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(16), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"80.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(17), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"80.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(18), R"({"enabled":true,"rsiMonth":"70.00","rsiWeek":"80.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(19), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"60.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(20), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"60.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(21), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"60.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(22), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"70.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(23), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"70.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(24), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"70.00","rsiDay":"80.00"})");
    ASSERT_EQ(variants.at(25), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"80.00","rsiDay":"60.00"})");
    ASSERT_EQ(variants.at(26), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"80.00","rsiDay":"70.00"})");
    ASSERT_EQ(variants.at(27), R"({"enabled":true,"rsiMonth":"80.00","rsiWeek":"80.00","rsiDay":"80.00"})");
}

TEST_F(Test_BuyDecision9Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_BuyDecision9Config, Test_setRsiMonth_and_getRsiMonth)
{
    config->setRsiMonth(0.1f);
    ASSERT_EQ(config->getRsiMonth(), 0.1f);

    config->setRsiMonth(0.5f);
    ASSERT_EQ(config->getRsiMonth(), 0.5f);
}

TEST_F(Test_BuyDecision9Config, Test_setRsiWeek_and_getRsiWeek)
{
    config->setRsiWeek(1.5f);
    ASSERT_EQ(config->getRsiWeek(), 1.5f);

    config->setRsiWeek(5.5f);
    ASSERT_EQ(config->getRsiWeek(), 5.5f);
}

TEST_F(Test_BuyDecision9Config, Test_setRsiDay_and_getRsiDay)
{
    config->setRsiDay(100);
    ASSERT_EQ(config->getRsiDay(), 100);

    config->setRsiDay(20);
    ASSERT_EQ(config->getRsiDay(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
