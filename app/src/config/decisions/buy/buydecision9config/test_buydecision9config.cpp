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
    config->setPriceRaise(2.5f);
    config->setOrderBookPositions(3);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             false);
    ASSERT_EQ(config->getPriceRaise(),         2.5f);
    ASSERT_EQ(config->getOrderBookPositions(), 3);
    ASSERT_EQ(config->getDuration(),           123);
    // clang-format on

    IBuyDecision9Config* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),              false);
    ASSERT_EQ(config->getPriceRaise(),          2.5f);
    ASSERT_EQ(config->getOrderBookPositions(),  3);
    ASSERT_EQ(config->getDuration(),            123);
    ASSERT_EQ(config2->isEnabled(),             false);
    ASSERT_EQ(config2->getPriceRaise(),         2.5f);
    ASSERT_EQ(config2->getOrderBookPositions(), 3);
    ASSERT_EQ(config2->getDuration(),           123);
    // clang-format on

    config2->deleteRecursively();
}

TEST_F(Test_BuyDecision9Config, Test_assign)
{
    BuyDecision9Config config2;

    config->setEnabled(false);
    config->setPriceRaise(2.5f);
    config->setOrderBookPositions(3);
    config->setDuration(123);

    config2.setEnabled(true);
    config2.setPriceRaise(1.6f);
    config2.setOrderBookPositions(4);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             false);
    ASSERT_EQ(config->getPriceRaise(),         2.5f);
    ASSERT_EQ(config->getOrderBookPositions(), 3);
    ASSERT_EQ(config->getDuration(),           123);
    ASSERT_EQ(config2.isEnabled(),             true);
    ASSERT_EQ(config2.getPriceRaise(),         1.6f);
    ASSERT_EQ(config2.getOrderBookPositions(), 4);
    ASSERT_EQ(config2.getDuration(),           321);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             true);
    ASSERT_EQ(config->getPriceRaise(),         1.6f);
    ASSERT_EQ(config->getOrderBookPositions(), 4);
    ASSERT_EQ(config->getDuration(),           321);
    ASSERT_EQ(config2.isEnabled(),             true);
    ASSERT_EQ(config2.getPriceRaise(),         1.6f);
    ASSERT_EQ(config2.getOrderBookPositions(), 4);
    ASSERT_EQ(config2.getDuration(),           321);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_makeDefault)
{
    config->setEnabled(false);
    config->setPriceRaise(2.5f);
    config->setOrderBookPositions(4);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             false);
    ASSERT_EQ(config->getPriceRaise(),         2.5f);
    ASSERT_EQ(config->getOrderBookPositions(), 4);
    ASSERT_EQ(config->getDuration(),           123);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             true);
    ASSERT_EQ(config->getPriceRaise(),         2.0f);
    ASSERT_EQ(config->getOrderBookPositions(), 10);
    ASSERT_EQ(config->getDuration(),           5);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_save)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setPriceRaise(2.5f);
    config->setOrderBookPositions(4);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             false);
    ASSERT_EQ(config->getPriceRaise(),         2.5f);
    ASSERT_EQ(config->getOrderBookPositions(), 4);
    ASSERT_EQ(config->getDuration(),           123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),            QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceRaise"),         QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/OrderBookPositions"), QVariant(4)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),           QVariant(123)));
    // clang-format on

    config->save(&settingsEditorMock, "BLAH");
}

TEST_F(Test_BuyDecision9Config, Test_load)
{
    const InSequence seq;

    config->setEnabled(false);
    config->setPriceRaise(2.5f);
    config->setOrderBookPositions(4);
    config->setDuration(123);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             false);
    ASSERT_EQ(config->getPriceRaise(),         2.5f);
    ASSERT_EQ(config->getOrderBookPositions(), 4);
    ASSERT_EQ(config->getDuration(),           123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),            QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceRaise"),         QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/OrderBookPositions"), QVariant(4))).WillOnce(Return(QVariant(8)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),           QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config->load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             true);
    ASSERT_EQ(config->getPriceRaise(),         1.7f);
    ASSERT_EQ(config->getOrderBookPositions(), 8);
    ASSERT_EQ(config->getDuration(),           321);
    // clang-format on
}

TEST_F(Test_BuyDecision9Config, Test_fromJsonObject)
{
    const QString content = R"({"enabled":true,"priceRaise":"1.70","orderBookPositions":8,"duration":321})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    config->fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             true);
    ASSERT_EQ(config->getPriceRaise(),         1.7f);
    ASSERT_EQ(config->getOrderBookPositions(), 8);
    ASSERT_EQ(config->getDuration(),           321);
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
    config->setPriceRaise(1.7f);
    config->setOrderBookPositions(8);
    config->setDuration(321);

    // clang-format off
    ASSERT_EQ(config->isEnabled(),             true);
    ASSERT_EQ(config->getPriceRaise(),         1.7f);
    ASSERT_EQ(config->getOrderBookPositions(), 8);
    ASSERT_EQ(config->getDuration(),           321);
    // clang-format on

    const QString content         = config->toJsonString();
    const QString expectedContent = R"({"enabled":true,"priceRaise":"1.70","orderBookPositions":8,"duration":321})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BuyDecision9Config, Test_variantsAsJson)
{
    QStringList variants = config->variantsAsJson();

    ASSERT_EQ(variants.size(), 61);
    ASSERT_EQ(variants.at(0), R"({"enabled":false})");
    ASSERT_EQ(variants.at(1), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":5,"duration":5})");
    ASSERT_EQ(variants.at(2), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":5,"duration":10})");
    ASSERT_EQ(variants.at(3), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":5,"duration":15})");
    ASSERT_EQ(variants.at(4), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":10,"duration":5})");
    ASSERT_EQ(variants.at(5), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":10,"duration":10})");
    ASSERT_EQ(variants.at(6), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":10,"duration":15})");
    ASSERT_EQ(variants.at(7), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":15,"duration":5})");
    ASSERT_EQ(variants.at(8), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":15,"duration":10})");
    ASSERT_EQ(variants.at(9), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":15,"duration":15})");
    ASSERT_EQ(variants.at(10), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":20,"duration":5})");
    ASSERT_EQ(variants.at(11), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":20,"duration":10})");
    ASSERT_EQ(variants.at(12), R"({"enabled":true,"priceRaise":"1.00","orderBookPositions":20,"duration":15})");
    ASSERT_EQ(variants.at(13), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":5,"duration":5})");
    ASSERT_EQ(variants.at(14), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":5,"duration":10})");
    ASSERT_EQ(variants.at(15), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":5,"duration":15})");
    ASSERT_EQ(variants.at(16), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":10,"duration":5})");
    ASSERT_EQ(variants.at(17), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":10,"duration":10})");
    ASSERT_EQ(variants.at(18), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":10,"duration":15})");
    ASSERT_EQ(variants.at(19), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":15,"duration":5})");
    ASSERT_EQ(variants.at(20), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":15,"duration":10})");
    ASSERT_EQ(variants.at(21), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":15,"duration":15})");
    ASSERT_EQ(variants.at(22), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":20,"duration":5})");
    ASSERT_EQ(variants.at(23), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":20,"duration":10})");
    ASSERT_EQ(variants.at(24), R"({"enabled":true,"priceRaise":"2.00","orderBookPositions":20,"duration":15})");
    ASSERT_EQ(variants.at(25), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":5,"duration":5})");
    ASSERT_EQ(variants.at(26), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":5,"duration":10})");
    ASSERT_EQ(variants.at(27), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":5,"duration":15})");
    ASSERT_EQ(variants.at(28), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":10,"duration":5})");
    ASSERT_EQ(variants.at(29), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":10,"duration":10})");
    ASSERT_EQ(variants.at(30), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":10,"duration":15})");
    ASSERT_EQ(variants.at(31), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":15,"duration":5})");
    ASSERT_EQ(variants.at(32), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":15,"duration":10})");
    ASSERT_EQ(variants.at(33), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":15,"duration":15})");
    ASSERT_EQ(variants.at(34), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":20,"duration":5})");
    ASSERT_EQ(variants.at(35), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":20,"duration":10})");
    ASSERT_EQ(variants.at(36), R"({"enabled":true,"priceRaise":"3.00","orderBookPositions":20,"duration":15})");
    ASSERT_EQ(variants.at(37), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":5,"duration":5})");
    ASSERT_EQ(variants.at(38), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":5,"duration":10})");
    ASSERT_EQ(variants.at(39), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":5,"duration":15})");
    ASSERT_EQ(variants.at(40), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":10,"duration":5})");
    ASSERT_EQ(variants.at(41), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":10,"duration":10})");
    ASSERT_EQ(variants.at(42), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":10,"duration":15})");
    ASSERT_EQ(variants.at(43), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":15,"duration":5})");
    ASSERT_EQ(variants.at(44), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":15,"duration":10})");
    ASSERT_EQ(variants.at(45), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":15,"duration":15})");
    ASSERT_EQ(variants.at(46), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":20,"duration":5})");
    ASSERT_EQ(variants.at(47), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":20,"duration":10})");
    ASSERT_EQ(variants.at(48), R"({"enabled":true,"priceRaise":"4.00","orderBookPositions":20,"duration":15})");
    ASSERT_EQ(variants.at(49), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":5,"duration":5})");
    ASSERT_EQ(variants.at(50), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":5,"duration":10})");
    ASSERT_EQ(variants.at(51), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":5,"duration":15})");
    ASSERT_EQ(variants.at(52), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":10,"duration":5})");
    ASSERT_EQ(variants.at(53), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":10,"duration":10})");
    ASSERT_EQ(variants.at(54), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":10,"duration":15})");
    ASSERT_EQ(variants.at(55), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":15,"duration":5})");
    ASSERT_EQ(variants.at(56), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":15,"duration":10})");
    ASSERT_EQ(variants.at(57), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":15,"duration":15})");
    ASSERT_EQ(variants.at(58), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":20,"duration":5})");
    ASSERT_EQ(variants.at(59), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":20,"duration":10})");
    ASSERT_EQ(variants.at(60), R"({"enabled":true,"priceRaise":"5.00","orderBookPositions":20,"duration":15})");
}

TEST_F(Test_BuyDecision9Config, Test_setEnabled_and_isEnabled)
{
    config->setEnabled(false);
    ASSERT_EQ(config->isEnabled(), false);

    config->setEnabled(true);
    ASSERT_EQ(config->isEnabled(), true);
}

TEST_F(Test_BuyDecision9Config, Test_setPriceRaise_and_getPriceRaise)
{
    config->setPriceRaise(0.1f);
    ASSERT_EQ(config->getPriceRaise(), 0.1f);

    config->setPriceRaise(0.5f);
    ASSERT_EQ(config->getPriceRaise(), 0.5f);
}

TEST_F(Test_BuyDecision9Config, Test_setOrderBookPositions_and_getOrderBookPositions)
{
    config->setOrderBookPositions(1);
    ASSERT_EQ(config->getOrderBookPositions(), 1);

    config->setOrderBookPositions(5);
    ASSERT_EQ(config->getOrderBookPositions(), 5);
}

TEST_F(Test_BuyDecision9Config, Test_setDuration_and_getDuration)
{
    config->setDuration(100);
    ASSERT_EQ(config->getDuration(), 100);

    config->setDuration(20);
    ASSERT_EQ(config->getDuration(), 20);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
