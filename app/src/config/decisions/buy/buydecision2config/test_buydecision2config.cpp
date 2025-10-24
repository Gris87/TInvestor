#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_BuyDecision2Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new BuyDecision2Config();
    }

    void TearDown() override
    {
        delete config;
    }

    BuyDecision2Config* config;
};



TEST_F(Test_BuyDecision2Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision2Config, Test_clone_and_deleteRecursively)
{
    config->setEnabled(false);
    config->setPriceFall(2.5f);
    config->setLoseYield(3.7f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getPriceFall(), 2.5f);
    ASSERT_EQ(config->getLoseYield(), 3.7f);
    ASSERT_EQ(config->getDuration(),  123);
    // clang-format on

    IBuyDecision2Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),     false);
    ASSERT_EQ(config->getPriceFall(),  2.5f);
    ASSERT_EQ(config->getLoseYield(),  3.7f);
    ASSERT_EQ(config->getDuration(),   123);
    ASSERT_EQ(config2->isEnabled(),    false);
    ASSERT_EQ(config2->getPriceFall(), 2.5f);
    ASSERT_EQ(config2->getLoseYield(), 3.7f);
    ASSERT_EQ(config2->getDuration(),  123);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_BuyDecision2Config, Test_assign)
{
    BuyDecision2Config config2;

    config->setEnabled(false);
    config->setPriceFall(2.5f);
    config->setLoseYield(3.7f);
    config->setDuration(123);

    config2.setEnabled(true);
    config2.setPriceFall(1.6f);
    config2.setLoseYield(4.2f);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getPriceFall(), 2.5f);
    ASSERT_EQ(config->getLoseYield(), 3.7f);
    ASSERT_EQ(config->getDuration(),  123);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceFall(), 1.6f);
    ASSERT_EQ(config2.getLoseYield(), 4.2f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 1.6f);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    ASSERT_EQ(config->getDuration(),  321);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceFall(), 1.6f);
    ASSERT_EQ(config2.getLoseYield(), 4.2f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on
}

TEST_F(Test_BuyDecision2Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setPriceFall(2.5f);
    config->setLoseYield(4.2f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getPriceFall(), 2.5f);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    ASSERT_EQ(config->getDuration(),  123);
    // clang-format on

    config->makeDefault(30);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 2.0f);
    ASSERT_EQ(config->getLoseYield(), 0.3f);
    ASSERT_EQ(config->getDuration(),  60);
    // clang-format on

    config->makeDefault(5);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 2.0f);
    ASSERT_EQ(config->getLoseYield(), 0.3f);
    ASSERT_EQ(config->getDuration(),  60);
    // clang-format on

    config->makeDefault(4);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 2.0f);
    ASSERT_EQ(config->getLoseYield(), 0.3f);
    ASSERT_EQ(config->getDuration(),  60);
    // clang-format on
}

TEST_F(Test_BuyDecision2Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setPriceFall(2.5f);
    config->setLoseYield(4.2f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getPriceFall(), 2.5f);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    ASSERT_EQ(config->getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceFall"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseYield"), QVariant(4.2f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),  QVariant(123)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_BuyDecision2Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setPriceFall(2.5f);
    config->setLoseYield(4.2f);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    false);
    ASSERT_EQ(config->getPriceFall(), 2.5f);
    ASSERT_EQ(config->getLoseYield(), 4.2f);
    ASSERT_EQ(config->getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceFall"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseYield"), QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),  QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 1.7f);
    ASSERT_EQ(config->getLoseYield(), 8.9f);
    ASSERT_EQ(config->getDuration(),  321);
    // clang-format on
}

TEST_F(Test_BuyDecision2Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"priceFall":"1.70","loseYield":"8.90","duration":321})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 1.7f);
    ASSERT_EQ(config->getLoseYield(), 8.9f);
    ASSERT_EQ(config->getDuration(),  321);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    config->fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_BuyDecision2Config, Test_toJsonString)
{
    config->setEnabled(true);
    config->setPriceFall(1.7f);
    config->setLoseYield(8.9f);
    config->setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),    true);
    ASSERT_EQ(config->getPriceFall(), 1.7f);
    ASSERT_EQ(config->getLoseYield(), 8.9f);
    ASSERT_EQ(config->getDuration(),  321);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"priceFall":"1.70","loseYield":"8.90","duration":321})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BuyDecision2Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 106);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":5})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":15})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":30})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":60})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":120})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":180})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.3","duration":240})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":5})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":15})");
    ASSERT_EQ(variants.at(10), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":30})");
    ASSERT_EQ(variants.at(11), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":60})");
    ASSERT_EQ(variants.at(12), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":120})");
    ASSERT_EQ(variants.at(13), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":180})");
    ASSERT_EQ(variants.at(14), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.5","duration":240})");
    ASSERT_EQ(variants.at(15), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":5})");
    ASSERT_EQ(variants.at(16), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":15})");
    ASSERT_EQ(variants.at(17), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":30})");
    ASSERT_EQ(variants.at(18), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":60})");
    ASSERT_EQ(variants.at(19), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":120})");
    ASSERT_EQ(variants.at(20), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":180})");
    ASSERT_EQ(variants.at(21), R"({"enabled":true,"priceFall":"2.00","loseYield":"0.7","duration":240})");
    ASSERT_EQ(variants.at(22), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":5})");
    ASSERT_EQ(variants.at(23), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":15})");
    ASSERT_EQ(variants.at(24), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":30})");
    ASSERT_EQ(variants.at(25), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":60})");
    ASSERT_EQ(variants.at(26), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":120})");
    ASSERT_EQ(variants.at(27), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":180})");
    ASSERT_EQ(variants.at(28), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.3","duration":240})");
    ASSERT_EQ(variants.at(29), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":5})");
    ASSERT_EQ(variants.at(30), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":15})");
    ASSERT_EQ(variants.at(31), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":30})");
    ASSERT_EQ(variants.at(32), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":60})");
    ASSERT_EQ(variants.at(33), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":120})");
    ASSERT_EQ(variants.at(34), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":180})");
    ASSERT_EQ(variants.at(35), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.5","duration":240})");
    ASSERT_EQ(variants.at(36), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":5})");
    ASSERT_EQ(variants.at(37), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":15})");
    ASSERT_EQ(variants.at(38), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":30})");
    ASSERT_EQ(variants.at(39), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":60})");
    ASSERT_EQ(variants.at(40), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":120})");
    ASSERT_EQ(variants.at(41), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":180})");
    ASSERT_EQ(variants.at(42), R"({"enabled":true,"priceFall":"2.50","loseYield":"0.7","duration":240})");
    ASSERT_EQ(variants.at(43), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":5})");
    ASSERT_EQ(variants.at(44), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":15})");
    ASSERT_EQ(variants.at(45), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":30})");
    ASSERT_EQ(variants.at(46), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":60})");
    ASSERT_EQ(variants.at(47), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":120})");
    ASSERT_EQ(variants.at(48), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":180})");
    ASSERT_EQ(variants.at(49), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.3","duration":240})");
    ASSERT_EQ(variants.at(50), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":5})");
    ASSERT_EQ(variants.at(51), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":15})");
    ASSERT_EQ(variants.at(52), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":30})");
    ASSERT_EQ(variants.at(53), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":60})");
    ASSERT_EQ(variants.at(54), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":120})");
    ASSERT_EQ(variants.at(55), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":180})");
    ASSERT_EQ(variants.at(56), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.5","duration":240})");
    ASSERT_EQ(variants.at(57), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":5})");
    ASSERT_EQ(variants.at(58), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":15})");
    ASSERT_EQ(variants.at(59), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":30})");
    ASSERT_EQ(variants.at(60), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":60})");
    ASSERT_EQ(variants.at(61), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":120})");
    ASSERT_EQ(variants.at(62), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":180})");
    ASSERT_EQ(variants.at(63), R"({"enabled":true,"priceFall":"3.00","loseYield":"0.7","duration":240})");
    ASSERT_EQ(variants.at(64), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":5})");
    ASSERT_EQ(variants.at(65), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":15})");
    ASSERT_EQ(variants.at(66), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":30})");
    ASSERT_EQ(variants.at(67), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":60})");
    ASSERT_EQ(variants.at(68), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":120})");
    ASSERT_EQ(variants.at(69), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":180})");
    ASSERT_EQ(variants.at(70), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.3","duration":240})");
    ASSERT_EQ(variants.at(71), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":5})");
    ASSERT_EQ(variants.at(72), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":15})");
    ASSERT_EQ(variants.at(73), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":30})");
    ASSERT_EQ(variants.at(74), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":60})");
    ASSERT_EQ(variants.at(75), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":120})");
    ASSERT_EQ(variants.at(76), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":180})");
    ASSERT_EQ(variants.at(77), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.5","duration":240})");
    ASSERT_EQ(variants.at(78), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":5})");
    ASSERT_EQ(variants.at(79), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":15})");
    ASSERT_EQ(variants.at(80), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":30})");
    ASSERT_EQ(variants.at(81), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":60})");
    ASSERT_EQ(variants.at(82), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":120})");
    ASSERT_EQ(variants.at(83), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":180})");
    ASSERT_EQ(variants.at(84), R"({"enabled":true,"priceFall":"3.50","loseYield":"0.7","duration":240})");
    ASSERT_EQ(variants.at(85), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":5})");
    ASSERT_EQ(variants.at(86), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":15})");
    ASSERT_EQ(variants.at(87), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":30})");
    ASSERT_EQ(variants.at(88), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":60})");
    ASSERT_EQ(variants.at(89), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":120})");
    ASSERT_EQ(variants.at(90), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":180})");
    ASSERT_EQ(variants.at(91), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.3","duration":240})");
    ASSERT_EQ(variants.at(92), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":5})");
    ASSERT_EQ(variants.at(93), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":15})");
    ASSERT_EQ(variants.at(94), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":30})");
    ASSERT_EQ(variants.at(95), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":60})");
    ASSERT_EQ(variants.at(96), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":120})");
    ASSERT_EQ(variants.at(97), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":180})");
    ASSERT_EQ(variants.at(98), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.5","duration":240})");
    ASSERT_EQ(variants.at(99), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":5})");
    ASSERT_EQ(variants.at(100), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":15})");
    ASSERT_EQ(variants.at(101), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":30})");
    ASSERT_EQ(variants.at(102), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":60})");
    ASSERT_EQ(variants.at(103), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":120})");
    ASSERT_EQ(variants.at(104), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":180})");
    ASSERT_EQ(variants.at(105), R"({"enabled":true,"priceFall":"4.00","loseYield":"0.7","duration":240})");
}

TEST_F(Test_BuyDecision2Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_BuyDecision2Config, Test_setPriceFall_and_getPriceFall)
{
    config->setPriceFall(0.1f);
    ASSERT_EQ(config->getPriceFall(), 0.1f);

    config->setPriceFall(0.5f);
    ASSERT_EQ(config->getPriceFall(), 0.5f);
}

TEST_F(Test_BuyDecision2Config, Test_setLoseYield_and_getLoseYield)
{
    config->setLoseYield(0.1f);
    ASSERT_EQ(config->getLoseYield(), 0.1f);

    config->setLoseYield(0.5f);
    ASSERT_EQ(config->getLoseYield(), 0.5f);
}

TEST_F(Test_BuyDecision2Config, Test_setDuration_and_getDuration)
{
    config->setDuration(100);
    ASSERT_EQ(config->getDuration(), 100);

    config->setDuration(20);
    ASSERT_EQ(config->getDuration(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
