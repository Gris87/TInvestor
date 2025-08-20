#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_BuyDecision1Config, Test_constructor_and_destructor)
{
    const BuyDecision1Config config;
}

TEST(Test_BuyDecision1Config, Test_clone_and_deleteRecursively)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    IBuyDecision1Config* config2 = config.clone();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getPriceFall(),   2.5f);
    ASSERT_EQ(config.getDuration(),    123);
    ASSERT_EQ(config2->isEnabled(),    false);
    ASSERT_EQ(config2->getPriceFall(), 2.5f);
    ASSERT_EQ(config2->getDuration(),  123);
    // clang-format on

    config2->deleteRecursively();
}

TEST(Test_BuyDecision1Config, Test_assign)
{
    BuyDecision1Config config;
    BuyDecision1Config config2;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    config2.setEnabled(true);
    config2.setPriceFall(1.6f);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getPriceFall(),  2.5f);
    ASSERT_EQ(config.getDuration(),   123);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceFall(), 1.6f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getPriceFall(),  1.6f);
    ASSERT_EQ(config.getDuration(),   321);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceFall(), 1.6f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on
}

TEST(Test_BuyDecision1Config, Test_makeDefault)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  30);
    // clang-format on
}

TEST(Test_BuyDecision1Config, Test_save)
{
    const InSequence seq;

    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceFall"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),  QVariant(123)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_BuyDecision1Config, Test_load)
{
    const InSequence seq;

    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceFall"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),  QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceFall(), 1.7f);
    ASSERT_EQ(config.getDuration(),  321);
    // clang-format on
}

TEST(Test_BuyDecision1Config, Test_fromJsonObject)
{
    BuyDecision1Config config;

    const QString content = R"({"enabled":true,"priceFall":"1.70","duration":321})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceFall(), 1.7f);
    ASSERT_EQ(config.getDuration(),  321);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST(Test_BuyDecision1Config, Test_toJsonString)
{
    BuyDecision1Config config;

    config.setEnabled(true);
    config.setPriceFall(1.7f);
    config.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceFall(), 1.7f);
    ASSERT_EQ(config.getDuration(),  321);
    // clang-format on

    const QString content         = config.toJsonString();
    const QString expectedContent = R"({"enabled":true,"priceFall":"1.70","duration":321})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_BuyDecision1Config, Test_variantsAsJson)
{
    BuyDecision1Config config;

    QStringList variants = config.variantsAsJson();

    ASSERT_EQ(variants.size(), 36);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"priceFall":"2.00","duration":5})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"priceFall":"2.00","duration":15})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"priceFall":"2.00","duration":30})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"priceFall":"2.00","duration":60})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"priceFall":"2.00","duration":120})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"priceFall":"2.00","duration":180})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"priceFall":"2.00","duration":240})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"priceFall":"2.50","duration":5})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"priceFall":"2.50","duration":15})");
    ASSERT_EQ(variants.at(10), R"({"enabled":true,"priceFall":"2.50","duration":30})");
    ASSERT_EQ(variants.at(11), R"({"enabled":true,"priceFall":"2.50","duration":60})");
    ASSERT_EQ(variants.at(12), R"({"enabled":true,"priceFall":"2.50","duration":120})");
    ASSERT_EQ(variants.at(13), R"({"enabled":true,"priceFall":"2.50","duration":180})");
    ASSERT_EQ(variants.at(14), R"({"enabled":true,"priceFall":"2.50","duration":240})");
    ASSERT_EQ(variants.at(15), R"({"enabled":true,"priceFall":"3.00","duration":5})");
    ASSERT_EQ(variants.at(16), R"({"enabled":true,"priceFall":"3.00","duration":15})");
    ASSERT_EQ(variants.at(17), R"({"enabled":true,"priceFall":"3.00","duration":30})");
    ASSERT_EQ(variants.at(18), R"({"enabled":true,"priceFall":"3.00","duration":60})");
    ASSERT_EQ(variants.at(19), R"({"enabled":true,"priceFall":"3.00","duration":120})");
    ASSERT_EQ(variants.at(20), R"({"enabled":true,"priceFall":"3.00","duration":180})");
    ASSERT_EQ(variants.at(21), R"({"enabled":true,"priceFall":"3.00","duration":240})");
    ASSERT_EQ(variants.at(22), R"({"enabled":true,"priceFall":"3.50","duration":5})");
    ASSERT_EQ(variants.at(23), R"({"enabled":true,"priceFall":"3.50","duration":15})");
    ASSERT_EQ(variants.at(24), R"({"enabled":true,"priceFall":"3.50","duration":30})");
    ASSERT_EQ(variants.at(25), R"({"enabled":true,"priceFall":"3.50","duration":60})");
    ASSERT_EQ(variants.at(26), R"({"enabled":true,"priceFall":"3.50","duration":120})");
    ASSERT_EQ(variants.at(27), R"({"enabled":true,"priceFall":"3.50","duration":180})");
    ASSERT_EQ(variants.at(28), R"({"enabled":true,"priceFall":"3.50","duration":240})");
    ASSERT_EQ(variants.at(29), R"({"enabled":true,"priceFall":"4.00","duration":5})");
    ASSERT_EQ(variants.at(30), R"({"enabled":true,"priceFall":"4.00","duration":15})");
    ASSERT_EQ(variants.at(31), R"({"enabled":true,"priceFall":"4.00","duration":30})");
    ASSERT_EQ(variants.at(32), R"({"enabled":true,"priceFall":"4.00","duration":60})");
    ASSERT_EQ(variants.at(33), R"({"enabled":true,"priceFall":"4.00","duration":120})");
    ASSERT_EQ(variants.at(34), R"({"enabled":true,"priceFall":"4.00","duration":180})");
    ASSERT_EQ(variants.at(35), R"({"enabled":true,"priceFall":"4.00","duration":240})");
}

TEST(Test_BuyDecision1Config, Test_setEnabled_and_isEnabled)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_BuyDecision1Config, Test_setPriceFall_and_getPriceFall)
{
    BuyDecision1Config config;

    config.setPriceFall(0.1f);
    ASSERT_EQ(config.getPriceFall(), 0.1f);

    config.setPriceFall(0.5f);
    ASSERT_EQ(config.getPriceFall(), 0.5f);
}

TEST(Test_BuyDecision1Config, Test_setDuration_and_getDuration)
{
    BuyDecision1Config config;

    config.setDuration(100);
    ASSERT_EQ(config.getDuration(), 100);

    config.setDuration(20);
    ASSERT_EQ(config.getDuration(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
