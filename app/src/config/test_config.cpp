#include "src/config/config.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        simulatorConfigMock = new StrictMock<DecisionMakerConfigMock>();
        autoPilotConfigMock = new StrictMock<DecisionMakerConfigMock>();

        config = new Config(simulatorConfigMock, autoPilotConfigMock);
    }

    void TearDown() override
    {
        delete config;
        delete simulatorConfigMock;
        delete autoPilotConfigMock;
    }

    Config*                              config;
    StrictMock<DecisionMakerConfigMock>* simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>* autoPilotConfigMock;
};



TEST_F(Test_Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_Config, Test_clone_and_deleteRecursively)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> simulatorConfigMock2;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock2;

    config->setAutorun(false);
    config->setCpuUsage("MINIMUM");
    config->setMakeDecisionTimeout(5);
    config->setTradeInNonWorkingHours(false);
    config->setTradeHugeBid(false);
    config->setHugeBid(3.0f);
    config->setHugeBidLimitStockPurchase(false);
    config->setHugeBidLimitStockPurchasePart(5.0f);
    config->setHugeBidLimitByTurnover(false);
    config->setHugeBidLimitByTurnoverPercent(0.7f);
    config->setTradeHugeSpread(false);
    config->setHugeSpread(3.0f);
    config->setHugeSpreadLimitStockPurchase(false);
    config->setHugeSpreadLimitStockPurchasePart(5.0f);
    config->setHugeSpreadLimitByTurnover(false);
    config->setHugeSpreadLimitByTurnoverPercent(0.7f);
    config->setTradeLiquidityEtfNightly(false);
    config->setLiquidityEtfRemainedPartNightly(7.0f);
    config->setLimitStockPurchase(false);
    config->setLimitStockPurchasePart(25.0f);
    config->setLimitByTurnover(false);
    config->setLimitByTurnoverPercent(3.7f);
    config->setLimitStockPurchaseNonWorkingHours(false);
    config->setLimitStockPurchasePartNonWorkingHours(15.0f);
    config->setLimitByTurnoverNonWorkingHours(false);
    config->setLimitByTurnoverPercentNonWorkingHours(2.5f);
    config->setStorageMonthLimit(36);
    config->setHighlightGoodOperations(false);
    config->setHighlightGoodOperationsYield(2.0f);
    config->setHighlightBadOperations(false);
    config->setHighlightBadOperationsLose(3.0f);
    config->setSimulatorConfigCommon(false);
    config->setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  false);
    ASSERT_EQ(config->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    EXPECT_CALL(*simulatorConfigMock, clone()).WillOnce(Return(&simulatorConfigMock2));
    EXPECT_CALL(*autoPilotConfigMock, clone()).WillOnce(Return(&autoPilotConfigMock2));
    EXPECT_CALL(simulatorConfigMock2, assign(simulatorConfigMock));
    EXPECT_CALL(autoPilotConfigMock2, assign(autoPilotConfigMock));

    IConfig* config2 = config->clone();

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  false);
    ASSERT_EQ(config->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    // clang-format off
    ASSERT_EQ(config2->isAutorun(),                                  false);
    ASSERT_EQ(config2->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config2->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config2->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config2->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config2->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config2->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config2->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config2->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config2->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config2->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config2->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config2->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config2->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config2->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config2->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config2->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config2->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config2->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config2->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config2->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config2->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config2->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config2->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config2->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config2->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config2->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config2->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config2->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config2->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config2->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config2->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config2->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    EXPECT_CALL(simulatorConfigMock2, deleteRecursively());
    EXPECT_CALL(autoPilotConfigMock2, deleteRecursively());

    config2->deleteRecursively();
}

TEST_F(Test_Config, Test_assign)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> simulatorConfigMock2;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock2;

    Config config2(&simulatorConfigMock2, &autoPilotConfigMock2);

    config->setAutorun(false);
    config->setCpuUsage("MINIMUM");
    config->setMakeDecisionTimeout(5);
    config->setTradeInNonWorkingHours(false);
    config->setTradeHugeBid(false);
    config->setHugeBid(3.0f);
    config->setHugeBidLimitStockPurchase(false);
    config->setHugeBidLimitStockPurchasePart(5.0f);
    config->setHugeBidLimitByTurnover(false);
    config->setHugeBidLimitByTurnoverPercent(0.7f);
    config->setTradeHugeSpread(false);
    config->setHugeSpread(3.0f);
    config->setHugeSpreadLimitStockPurchase(false);
    config->setHugeSpreadLimitStockPurchasePart(5.0f);
    config->setHugeSpreadLimitByTurnover(false);
    config->setHugeSpreadLimitByTurnoverPercent(0.7f);
    config->setTradeLiquidityEtfNightly(false);
    config->setLiquidityEtfRemainedPartNightly(7.0f);
    config->setLimitStockPurchase(false);
    config->setLimitStockPurchasePart(25.0f);
    config->setLimitByTurnover(false);
    config->setLimitByTurnoverPercent(3.7f);
    config->setLimitStockPurchaseNonWorkingHours(false);
    config->setLimitStockPurchasePartNonWorkingHours(15.0f);
    config->setLimitByTurnoverNonWorkingHours(false);
    config->setLimitByTurnoverPercentNonWorkingHours(2.5f);
    config->setStorageMonthLimit(36);
    config->setHighlightGoodOperations(false);
    config->setHighlightGoodOperationsYield(2.0f);
    config->setHighlightBadOperations(false);
    config->setHighlightBadOperationsLose(3.0f);
    config->setSimulatorConfigCommon(false);
    config->setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  false);
    ASSERT_EQ(config->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    config2.setAutorun(true);
    config2.setCpuUsage("OPTIMAL");
    config2.setMakeDecisionTimeout(30);
    config2.setTradeInNonWorkingHours(true);
    config2.setTradeHugeBid(true);
    config2.setHugeBid(4.5f);
    config2.setHugeBidLimitStockPurchase(true);
    config2.setHugeBidLimitStockPurchasePart(3.5f);
    config2.setHugeBidLimitByTurnover(true);
    config2.setHugeBidLimitByTurnoverPercent(1.7f);
    config2.setTradeHugeSpread(true);
    config2.setHugeSpread(4.5f);
    config2.setHugeSpreadLimitStockPurchase(true);
    config2.setHugeSpreadLimitStockPurchasePart(3.5f);
    config2.setHugeSpreadLimitByTurnover(true);
    config2.setHugeSpreadLimitByTurnoverPercent(1.7f);
    config2.setTradeLiquidityEtfNightly(true);
    config2.setLiquidityEtfRemainedPartNightly(17.0f);
    config2.setLimitStockPurchase(true);
    config2.setLimitStockPurchasePart(5.0f);
    config2.setLimitByTurnover(true);
    config2.setLimitByTurnoverPercent(7.5f);
    config2.setLimitStockPurchaseNonWorkingHours(true);
    config2.setLimitStockPurchasePartNonWorkingHours(35.0f);
    config2.setLimitByTurnoverNonWorkingHours(true);
    config2.setLimitByTurnoverPercentNonWorkingHours(37.5f);
    config2.setStorageMonthLimit(12);
    config2.setHighlightGoodOperations(true);
    config2.setHighlightGoodOperationsYield(7.0f);
    config2.setHighlightBadOperations(true);
    config2.setHighlightBadOperationsLose(5.0f);
    config2.setSimulatorConfigCommon(true);
    config2.setAutoPilotConfigCommon(false);

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),                                  true);
    ASSERT_EQ(config2.getCpuUsage(),                                "OPTIMAL");
    ASSERT_EQ(config2.getMakeDecisionTimeout(),                     30);
    ASSERT_EQ(config2.isTradeInNonWorkingHours(),                   true);
    ASSERT_EQ(config2.isTradeHugeBid(),                             true);
    ASSERT_NEAR(config2.getHugeBid(),                               4.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeBidLimitStockPurchase(),                true);
    ASSERT_NEAR(config2.getHugeBidLimitStockPurchasePart(),         3.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeBidLimitByTurnover(),                   true);
    ASSERT_NEAR(config2.getHugeBidLimitByTurnoverPercent(),         1.7f, 0.0001f);
    ASSERT_EQ(config2.isTradeHugeSpread(),                          true);
    ASSERT_NEAR(config2.getHugeSpread(),                            4.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeSpreadLimitStockPurchase(),             true);
    ASSERT_NEAR(config2.getHugeSpreadLimitStockPurchasePart(),      3.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeSpreadLimitByTurnover(),                true);
    ASSERT_NEAR(config2.getHugeSpreadLimitByTurnoverPercent(),      1.7f, 0.0001f);
    ASSERT_EQ(config2.isTradeLiquidityEtfNightly(),                 true);
    ASSERT_NEAR(config2.getLiquidityEtfRemainedPartNightly(),       17.0f, 0.0001f);
    ASSERT_EQ(config2.isLimitStockPurchase(),                       true);
    ASSERT_NEAR(config2.getLimitStockPurchasePart(),                5.0f, 0.0001f);
    ASSERT_EQ(config2.isLimitByTurnover(),                          true);
    ASSERT_NEAR(config2.getLimitByTurnoverPercent(),                7.5f, 0.0001f);
    ASSERT_EQ(config2.isLimitStockPurchaseNonWorkingHours(),        true);
    ASSERT_NEAR(config2.getLimitStockPurchasePartNonWorkingHours(), 35.0f, 0.0001f);
    ASSERT_EQ(config2.isLimitByTurnoverNonWorkingHours(),           true);
    ASSERT_NEAR(config2.getLimitByTurnoverPercentNonWorkingHours(), 37.5f, 0.0001f);
    ASSERT_EQ(config2.getStorageMonthLimit(),                       12);
    ASSERT_EQ(config2.isHighlightGoodOperations(),                  true);
    ASSERT_NEAR(config2.getHighlightGoodOperationsYield(),          7.0f, 0.0001f);
    ASSERT_EQ(config2.isHighlightBadOperations(),                   true);
    ASSERT_NEAR(config2.getHighlightBadOperationsLose(),            5.0f, 0.0001f);
    ASSERT_EQ(config2.isSimulatorConfigCommon(),                    true);
    ASSERT_EQ(config2.isAutoPilotConfigCommon(),                    false);
    // clang-format on

    EXPECT_CALL(*simulatorConfigMock, assign(&simulatorConfigMock2));
    EXPECT_CALL(*autoPilotConfigMock, assign(&autoPilotConfigMock2));

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  true);
    ASSERT_EQ(config->getCpuUsage(),                                "OPTIMAL");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     30);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   true);
    ASSERT_EQ(config->isTradeHugeBid(),                             true);
    ASSERT_NEAR(config->getHugeBid(),                               4.5f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                true);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         3.5f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   true);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         1.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          true);
    ASSERT_NEAR(config->getHugeSpread(),                            4.5f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             true);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      3.5f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                true);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      1.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 true);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       17.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       true);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                5.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          true);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                7.5f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        true);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 35.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           true);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 37.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       12);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  true);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          7.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   true);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            5.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    true);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    false);
    // clang-format on

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),                                  true);
    ASSERT_EQ(config2.getCpuUsage(),                                "OPTIMAL");
    ASSERT_EQ(config2.getMakeDecisionTimeout(),                     30);
    ASSERT_EQ(config2.isTradeInNonWorkingHours(),                   true);
    ASSERT_EQ(config2.isTradeHugeBid(),                             true);
    ASSERT_NEAR(config2.getHugeBid(),                               4.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeBidLimitStockPurchase(),                true);
    ASSERT_NEAR(config2.getHugeBidLimitStockPurchasePart(),         3.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeBidLimitByTurnover(),                   true);
    ASSERT_NEAR(config2.getHugeBidLimitByTurnoverPercent(),         1.7f, 0.0001f);
    ASSERT_EQ(config2.isTradeHugeSpread(),                          true);
    ASSERT_NEAR(config2.getHugeSpread(),                            4.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeSpreadLimitStockPurchase(),             true);
    ASSERT_NEAR(config2.getHugeSpreadLimitStockPurchasePart(),      3.5f, 0.0001f);
    ASSERT_EQ(config2.isHugeSpreadLimitByTurnover(),                true);
    ASSERT_NEAR(config2.getHugeSpreadLimitByTurnoverPercent(),      1.7f, 0.0001f);
    ASSERT_EQ(config2.isTradeLiquidityEtfNightly(),                 true);
    ASSERT_NEAR(config2.getLiquidityEtfRemainedPartNightly(),       17.0f, 0.0001f);
    ASSERT_EQ(config2.isLimitStockPurchase(),                       true);
    ASSERT_NEAR(config2.getLimitStockPurchasePart(),                5.0f, 0.0001f);
    ASSERT_EQ(config2.isLimitByTurnover(),                          true);
    ASSERT_NEAR(config2.getLimitByTurnoverPercent(),                7.5f, 0.0001f);
    ASSERT_EQ(config2.isLimitStockPurchaseNonWorkingHours(),        true);
    ASSERT_NEAR(config2.getLimitStockPurchasePartNonWorkingHours(), 35.0f, 0.0001f);
    ASSERT_EQ(config2.isLimitByTurnoverNonWorkingHours(),           true);
    ASSERT_NEAR(config2.getLimitByTurnoverPercentNonWorkingHours(), 37.5f, 0.0001f);
    ASSERT_EQ(config2.getStorageMonthLimit(),                       12);
    ASSERT_EQ(config2.isHighlightGoodOperations(),                  true);
    ASSERT_NEAR(config2.getHighlightGoodOperationsYield(),          7.0f, 0.0001f);
    ASSERT_EQ(config2.isHighlightBadOperations(),                   true);
    ASSERT_NEAR(config2.getHighlightBadOperationsLose(),            5.0f, 0.0001f);
    ASSERT_EQ(config2.isSimulatorConfigCommon(),                    true);
    ASSERT_EQ(config2.isAutoPilotConfigCommon(),                    false);
    // clang-format on
}

TEST_F(Test_Config, Test_makeDefault)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setCpuUsage("MINIMUM");
    config->setMakeDecisionTimeout(5);
    config->setTradeInNonWorkingHours(false);
    config->setTradeHugeBid(false);
    config->setHugeBid(3.0f);
    config->setHugeBidLimitStockPurchase(false);
    config->setHugeBidLimitStockPurchasePart(5.0f);
    config->setHugeBidLimitByTurnover(false);
    config->setHugeBidLimitByTurnoverPercent(0.7f);
    config->setTradeHugeSpread(false);
    config->setHugeSpread(3.0f);
    config->setHugeSpreadLimitStockPurchase(false);
    config->setHugeSpreadLimitStockPurchasePart(15.0f);
    config->setHugeSpreadLimitByTurnover(false);
    config->setHugeSpreadLimitByTurnoverPercent(0.7f);
    config->setTradeLiquidityEtfNightly(false);
    config->setLiquidityEtfRemainedPartNightly(7.0f);
    config->setLimitStockPurchase(false);
    config->setLimitStockPurchasePart(25.0f);
    config->setLimitByTurnover(false);
    config->setLimitByTurnoverPercent(3.7f);
    config->setLimitStockPurchaseNonWorkingHours(true);
    config->setLimitStockPurchasePartNonWorkingHours(15.0f);
    config->setLimitByTurnoverNonWorkingHours(false);
    config->setLimitByTurnoverPercentNonWorkingHours(2.5f);
    config->setStorageMonthLimit(36);
    config->setHighlightGoodOperations(false);
    config->setHighlightGoodOperationsYield(2.0f);
    config->setHighlightBadOperations(false);
    config->setHighlightBadOperationsLose(3.0f);
    config->setSimulatorConfigCommon(false);
    config->setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  false);
    ASSERT_EQ(config->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      15.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        true);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    EXPECT_CALL(*simulatorConfigMock, makeDefault(4));
    EXPECT_CALL(*autoPilotConfigMock, makeDefault(4));

    config->makeDefault(0.04f);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  true);
    ASSERT_EQ(config->getCpuUsage(),                                "MAXIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     1);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   true);
    ASSERT_EQ(config->isTradeHugeBid(),                             true);
    ASSERT_NEAR(config->getHugeBid(),                               2.5f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                true);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         1.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   true);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         1.0f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          true);
    ASSERT_NEAR(config->getHugeSpread(),                            0.7f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             true);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                true);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      1.0f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 true);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       5.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       true);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                30.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          true);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                5.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 1.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           true);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 1.0f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       12);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  true);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          1.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   true);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            2.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    true);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    false);
    // clang-format on
}

TEST_F(Test_Config, Test_save)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setCpuUsage("MINIMUM");
    config->setMakeDecisionTimeout(5);
    config->setTradeInNonWorkingHours(false);
    config->setTradeHugeBid(false);
    config->setHugeBid(3.0f);
    config->setHugeBidLimitStockPurchase(false);
    config->setHugeBidLimitStockPurchasePart(5.0f);
    config->setHugeBidLimitByTurnover(false);
    config->setHugeBidLimitByTurnoverPercent(0.7f);
    config->setTradeHugeSpread(false);
    config->setHugeSpread(3.0f);
    config->setHugeSpreadLimitStockPurchase(false);
    config->setHugeSpreadLimitStockPurchasePart(5.0f);
    config->setHugeSpreadLimitByTurnover(false);
    config->setHugeSpreadLimitByTurnoverPercent(0.7f);
    config->setTradeLiquidityEtfNightly(false);
    config->setLiquidityEtfRemainedPartNightly(7.0f);
    config->setLimitStockPurchase(false);
    config->setLimitStockPurchasePart(25.0f);
    config->setLimitByTurnover(false);
    config->setLimitByTurnoverPercent(3.7f);
    config->setLimitStockPurchaseNonWorkingHours(false);
    config->setLimitStockPurchasePartNonWorkingHours(15.0f);
    config->setLimitByTurnoverNonWorkingHours(false);
    config->setLimitByTurnoverPercentNonWorkingHours(2.5f);
    config->setStorageMonthLimit(36);
    config->setHighlightGoodOperations(false);
    config->setHighlightGoodOperationsYield(2.0f);
    config->setHighlightBadOperations(false);
    config->setHighlightBadOperationsLose(3.0f);
    config->setSimulatorConfigCommon(false);
    config->setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  false);
    ASSERT_EQ(config->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(*simulatorConfigMock, save(&settingsEditorMock, QString("Config/Simulator")));
    EXPECT_CALL(*autoPilotConfigMock, save(&settingsEditorMock, QString("Config/AutoPilot")));

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/Autorun"),                               QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/CpuUsage"),                              QVariant("MINIMUM")));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/MakeDecisionTimeout"),                   QVariant(5)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/TradeInNonWorkingHours"),                QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/TradeHugeBid"),                          QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeBid"),                               QVariant(3.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeBidLimitStockPurchase"),             QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeBidLimitStockPurchasePart"),         QVariant(5.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeBidLimitByTurnover"),                QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeBidLimitByTurnoverPercent"),         QVariant(0.7f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/TradeHugeSpread"),                       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeSpread"),                            QVariant(3.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeSpreadLimitStockPurchase"),          QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeSpreadLimitStockPurchasePart"),      QVariant(5.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeSpreadLimitByTurnover"),             QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HugeSpreadLimitByTurnoverPercent"),      QVariant(0.7f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/TradeLiquidityEtfNightly"),              QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LiquidityEtfRemainedPartNightly"),       QVariant(7.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitStockPurchase"),                    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitStockPurchasePart"),                QVariant(25.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitByTurnover"),                       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitByTurnoverPercent"),                QVariant(3.7f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitStockPurchaseNonWorkingHours"),     QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitStockPurchasePartNonWorkingHours"), QVariant(15.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitByTurnoverNonWorkingHours"),        QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitByTurnoverPercentNonWorkingHours"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/StorageMonthLimit"),                     QVariant(36)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HighlightGoodOperations"),               QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HighlightGoodOperationsYield"),          QVariant(2.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HighlightBadOperations"),                QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/HighlightBadOperationsLose"),            QVariant(3.0f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/SimulatorConfigCommon"),                 QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/AutoPilotConfigCommon"),                 QVariant(true)));
    // clang-format on

    config->save(&settingsEditorMock);
}

TEST_F(Test_Config, Test_load)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setCpuUsage("MINIMUM");
    config->setMakeDecisionTimeout(5);
    config->setTradeInNonWorkingHours(false);
    config->setTradeHugeBid(false);
    config->setHugeBid(3.0f);
    config->setHugeBidLimitStockPurchase(false);
    config->setHugeBidLimitStockPurchasePart(5.0f);
    config->setHugeBidLimitByTurnover(false);
    config->setHugeBidLimitByTurnoverPercent(0.7f);
    config->setTradeHugeSpread(false);
    config->setHugeSpread(3.0f);
    config->setHugeSpreadLimitStockPurchase(false);
    config->setHugeSpreadLimitStockPurchasePart(5.0f);
    config->setHugeSpreadLimitByTurnover(false);
    config->setHugeSpreadLimitByTurnoverPercent(0.7f);
    config->setTradeLiquidityEtfNightly(false);
    config->setLiquidityEtfRemainedPartNightly(7.0f);
    config->setLimitStockPurchase(false);
    config->setLimitStockPurchasePart(25.0f);
    config->setLimitByTurnover(false);
    config->setLimitByTurnoverPercent(3.7f);
    config->setLimitStockPurchaseNonWorkingHours(false);
    config->setLimitStockPurchasePartNonWorkingHours(15.0f);
    config->setLimitByTurnoverNonWorkingHours(false);
    config->setLimitByTurnoverPercentNonWorkingHours(2.5f);
    config->setStorageMonthLimit(36);
    config->setHighlightGoodOperations(false);
    config->setHighlightGoodOperationsYield(2.0f);
    config->setHighlightBadOperations(false);
    config->setHighlightBadOperationsLose(3.0f);
    config->setSimulatorConfigCommon(false);
    config->setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  false);
    ASSERT_EQ(config->getCpuUsage(),                                "MINIMUM");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     5);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   false);
    ASSERT_EQ(config->isTradeHugeBid(),                             false);
    ASSERT_NEAR(config->getHugeBid(),                               3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                false);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   false);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          false);
    ASSERT_NEAR(config->getHugeSpread(),                            3.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             false);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      5.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                false);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      0.7f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 false);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       7.0f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       false);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                25.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          false);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                3.7f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        false);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 15.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           false);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 2.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       36);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  false);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          2.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   false);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            3.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    true);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(*simulatorConfigMock, load(&settingsEditorMock, QString("Config/Simulator")));
    EXPECT_CALL(*autoPilotConfigMock, load(&settingsEditorMock, QString("Config/AutoPilot")));

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("Config/Autorun"),                               QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/CpuUsage"),                              QVariant("MINIMUM"))).WillOnce(Return(QVariant("OPTIMAL")));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/MakeDecisionTimeout"),                   QVariant(5))).WillOnce(Return(QVariant(30)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/TradeInNonWorkingHours"),                QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/TradeHugeBid"),                          QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeBid"),                               QVariant(3.0f))).WillOnce(Return(QVariant(9.0f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeBidLimitStockPurchase"),             QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeBidLimitStockPurchasePart"),         QVariant(5.0f))).WillOnce(Return(QVariant(1.9f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeBidLimitByTurnover"),                QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeBidLimitByTurnoverPercent"),         QVariant(0.7f))).WillOnce(Return(QVariant(2.4f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/TradeHugeSpread"),                       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeSpread"),                            QVariant(3.0f))).WillOnce(Return(QVariant(9.0f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeSpreadLimitStockPurchase"),          QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeSpreadLimitStockPurchasePart"),      QVariant(5.0f))).WillOnce(Return(QVariant(1.9f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeSpreadLimitByTurnover"),             QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HugeSpreadLimitByTurnoverPercent"),      QVariant(0.7f))).WillOnce(Return(QVariant(2.4f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/TradeLiquidityEtfNightly"),              QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LiquidityEtfRemainedPartNightly"),       QVariant(7.0f))).WillOnce(Return(QVariant(4.5f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitStockPurchase"),                    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitStockPurchasePart"),                QVariant(25.0f))).WillOnce(Return(QVariant(5.0f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitByTurnover"),                       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitByTurnoverPercent"),                QVariant(3.7f))).WillOnce(Return(QVariant(7.5f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitStockPurchaseNonWorkingHours"),     QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitStockPurchasePartNonWorkingHours"), QVariant(15.0f))).WillOnce(Return(QVariant(35.0f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitByTurnoverNonWorkingHours"),        QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitByTurnoverPercentNonWorkingHours"), QVariant(2.5f))).WillOnce(Return(QVariant(17.5f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/StorageMonthLimit"),                     QVariant(36))).WillOnce(Return(QVariant(12)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HighlightGoodOperations"),               QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HighlightGoodOperationsYield"),          QVariant(2.0f))).WillOnce(Return(QVariant(7.0f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HighlightBadOperations"),                QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/HighlightBadOperationsLose"),            QVariant(3.0f))).WillOnce(Return(QVariant(5.0f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/SimulatorConfigCommon"),                 QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/AutoPilotConfigCommon"),                 QVariant(true))).WillOnce(Return(QVariant(false)));
    // clang-format on

    config->load(&settingsEditorMock);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),                                  true);
    ASSERT_EQ(config->getCpuUsage(),                                "OPTIMAL");
    ASSERT_EQ(config->getMakeDecisionTimeout(),                     30);
    ASSERT_EQ(config->isTradeInNonWorkingHours(),                   true);
    ASSERT_EQ(config->isTradeHugeBid(),                             true);
    ASSERT_NEAR(config->getHugeBid(),                               9.0f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(),                true);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(),         1.9f, 0.0001f);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(),                   true);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(),         2.4f, 0.0001f);
    ASSERT_EQ(config->isTradeHugeSpread(),                          true);
    ASSERT_NEAR(config->getHugeSpread(),                            9.0f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(),             true);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(),      1.9f, 0.0001f);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(),                true);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(),      2.4f, 0.0001f);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(),                 true);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(),       4.5f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchase(),                       true);
    ASSERT_NEAR(config->getLimitStockPurchasePart(),                5.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnover(),                          true);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(),                7.5f, 0.0001f);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(),        true);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 35.0f, 0.0001f);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(),           true);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 17.5f, 0.0001f);
    ASSERT_EQ(config->getStorageMonthLimit(),                       12);
    ASSERT_EQ(config->isHighlightGoodOperations(),                  true);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(),          7.0f, 0.0001f);
    ASSERT_EQ(config->isHighlightBadOperations(),                   true);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(),            5.0f, 0.0001f);
    ASSERT_EQ(config->isSimulatorConfigCommon(),                    true);
    ASSERT_EQ(config->isAutoPilotConfigCommon(),                    false);
    // clang-format on
}

TEST_F(Test_Config, Test_getSimulatorConfig)
{
    ASSERT_EQ(config->getSimulatorConfig(), simulatorConfigMock);
}

TEST_F(Test_Config, Test_getAutoPilotConfig)
{
    ASSERT_EQ(config->getAutoPilotConfig(), autoPilotConfigMock);
}

TEST_F(Test_Config, Test_setAutorun_and_isAutorun)
{
    config->setAutorun(false);
    ASSERT_EQ(config->isAutorun(), false);

    config->setAutorun(true);
    ASSERT_EQ(config->isAutorun(), true);
}

TEST_F(Test_Config, Test_setCpuUsage_and_getCpuUsage)
{
    config->setCpuUsage("MINIMUM");
    ASSERT_EQ(config->getCpuUsage(), "MINIMUM");

    config->setCpuUsage("OPTIMAL");
    ASSERT_EQ(config->getCpuUsage(), "OPTIMAL");
}

TEST_F(Test_Config, Test_setMakeDecisionTimeout_and_getMakeDecisionTimeout)
{
    config->setMakeDecisionTimeout(1);
    ASSERT_EQ(config->getMakeDecisionTimeout(), 1);

    config->setMakeDecisionTimeout(2);
    ASSERT_EQ(config->getMakeDecisionTimeout(), 2);
}

TEST_F(Test_Config, Test_setTradeInNonWorkingHours_and_isTradeInNonWorkingHours)
{
    config->setTradeInNonWorkingHours(false);
    ASSERT_EQ(config->isTradeInNonWorkingHours(), false);

    config->setTradeInNonWorkingHours(true);
    ASSERT_EQ(config->isTradeInNonWorkingHours(), true);
}

TEST_F(Test_Config, Test_setTradeHugeBid_and_isTradeHugeBid)
{
    config->setTradeHugeBid(false);
    ASSERT_EQ(config->isTradeHugeBid(), false);

    config->setTradeHugeBid(true);
    ASSERT_EQ(config->isTradeHugeBid(), true);
}

TEST_F(Test_Config, Test_setHugeBid_and_getHugeBid)
{
    config->setHugeBid(1.0f);
    ASSERT_NEAR(config->getHugeBid(), 1.0f, 0.0001f);

    config->setHugeBid(5.0f);
    ASSERT_NEAR(config->getHugeBid(), 5.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setHugeBidLimitStockPurchase_and_isHugeBidLimitStockPurchase)
{
    config->setHugeBidLimitStockPurchase(false);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(), false);

    config->setHugeBidLimitStockPurchase(true);
    ASSERT_EQ(config->isHugeBidLimitStockPurchase(), true);
}

TEST_F(Test_Config, Test_setHugeBidLimitStockPurchasePart_and_getHugeBidLimitStockPurchasePart)
{
    config->setHugeBidLimitStockPurchasePart(1.0f);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(), 1.0f, 0.0001f);

    config->setHugeBidLimitStockPurchasePart(5.0f);
    ASSERT_NEAR(config->getHugeBidLimitStockPurchasePart(), 5.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setHugeBidLimitByTurnover_and_isHugeBidLimitByTurnover)
{
    config->setHugeBidLimitByTurnover(false);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(), false);

    config->setHugeBidLimitByTurnover(true);
    ASSERT_EQ(config->isHugeBidLimitByTurnover(), true);
}

TEST_F(Test_Config, Test_setHugeBidLimitByTurnoverPercent_and_getHugeBidLimitByTurnoverPercent)
{
    config->setHugeBidLimitByTurnoverPercent(10.0f);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(), 10.0f, 0.0001f);

    config->setHugeBidLimitByTurnoverPercent(50.0f);
    ASSERT_NEAR(config->getHugeBidLimitByTurnoverPercent(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setTradeHugeSpread_and_isTradeHugeSpread)
{
    config->setTradeHugeSpread(false);
    ASSERT_EQ(config->isTradeHugeSpread(), false);

    config->setTradeHugeSpread(true);
    ASSERT_EQ(config->isTradeHugeSpread(), true);
}

TEST_F(Test_Config, Test_setHugeSpread_and_getHugeSpread)
{
    config->setHugeSpread(1.0f);
    ASSERT_NEAR(config->getHugeSpread(), 1.0f, 0.0001f);

    config->setHugeSpread(5.0f);
    ASSERT_NEAR(config->getHugeSpread(), 5.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setHugeSpreadLimitStockPurchase_and_isHugeSpreadLimitStockPurchase)
{
    config->setHugeSpreadLimitStockPurchase(false);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(), false);

    config->setHugeSpreadLimitStockPurchase(true);
    ASSERT_EQ(config->isHugeSpreadLimitStockPurchase(), true);
}

TEST_F(Test_Config, Test_setHugeSpreadLimitStockPurchasePart_and_getHugeSpreadLimitStockPurchasePart)
{
    config->setHugeSpreadLimitStockPurchasePart(1.0f);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(), 1.0f, 0.0001f);

    config->setHugeSpreadLimitStockPurchasePart(5.0f);
    ASSERT_NEAR(config->getHugeSpreadLimitStockPurchasePart(), 5.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setHugeSpreadLimitByTurnover_and_isHugeSpreadLimitByTurnover)
{
    config->setHugeSpreadLimitByTurnover(false);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(), false);

    config->setHugeSpreadLimitByTurnover(true);
    ASSERT_EQ(config->isHugeSpreadLimitByTurnover(), true);
}

TEST_F(Test_Config, Test_setHugeSpreadLimitByTurnoverPercent_and_getHugeSpreadLimitByTurnoverPercent)
{
    config->setHugeSpreadLimitByTurnoverPercent(10.0f);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(), 10.0f, 0.0001f);

    config->setHugeSpreadLimitByTurnoverPercent(50.0f);
    ASSERT_NEAR(config->getHugeSpreadLimitByTurnoverPercent(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setTradeLiquidityEtfNightly_and_isTradeLiquidityEtfNightly)
{
    config->setTradeLiquidityEtfNightly(false);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(), false);

    config->setTradeLiquidityEtfNightly(true);
    ASSERT_EQ(config->isTradeLiquidityEtfNightly(), true);
}

TEST_F(Test_Config, Test_setLiquidityEtfRemainedPartNightly_and_getLiquidityEtfRemainedPartNightly)
{
    config->setLiquidityEtfRemainedPartNightly(10.0f);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(), 10.0f, 0.0001f);

    config->setLiquidityEtfRemainedPartNightly(50.0f);
    ASSERT_NEAR(config->getLiquidityEtfRemainedPartNightly(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setLimitStockPurchase_and_isLimitStockPurchase)
{
    config->setLimitStockPurchase(false);
    ASSERT_EQ(config->isLimitStockPurchase(), false);

    config->setLimitStockPurchase(true);
    ASSERT_EQ(config->isLimitStockPurchase(), true);
}

TEST_F(Test_Config, Test_setLimitStockPurchasePart_and_getLimitStockPurchasePart)
{
    config->setLimitStockPurchasePart(1.0f);
    ASSERT_NEAR(config->getLimitStockPurchasePart(), 1.0f, 0.0001f);

    config->setLimitStockPurchasePart(5.0f);
    ASSERT_NEAR(config->getLimitStockPurchasePart(), 5.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setLimitByTurnover_and_isLimitByTurnover)
{
    config->setLimitByTurnover(false);
    ASSERT_EQ(config->isLimitByTurnover(), false);

    config->setLimitByTurnover(true);
    ASSERT_EQ(config->isLimitByTurnover(), true);
}

TEST_F(Test_Config, Test_setLimitByTurnoverPercent_and_getLimitByTurnoverPercent)
{
    config->setLimitByTurnoverPercent(10.0f);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(), 10.0f, 0.0001f);

    config->setLimitByTurnoverPercent(50.0f);
    ASSERT_NEAR(config->getLimitByTurnoverPercent(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setLimitStockPurchaseNonWorkingHours_and_isLimitStockPurchaseNonWorkingHours)
{
    config->setLimitStockPurchaseNonWorkingHours(false);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(), false);

    config->setLimitStockPurchaseNonWorkingHours(true);
    ASSERT_EQ(config->isLimitStockPurchaseNonWorkingHours(), true);
}

TEST_F(Test_Config, Test_setLimitStockPurchasePartNonWorkingHours_and_getLimitStockPurchasePartNonWorkingHours)
{
    config->setLimitStockPurchasePartNonWorkingHours(1.0f);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 1.0f, 0.0001f);

    config->setLimitStockPurchasePartNonWorkingHours(5.0f);
    ASSERT_NEAR(config->getLimitStockPurchasePartNonWorkingHours(), 5.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setLimitByTurnoverNonWorkingHours_and_isLimitByTurnoverNonWorkingHours)
{
    config->setLimitByTurnoverNonWorkingHours(false);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(), false);

    config->setLimitByTurnoverNonWorkingHours(true);
    ASSERT_EQ(config->isLimitByTurnoverNonWorkingHours(), true);
}

TEST_F(Test_Config, Test_setLimitByTurnoverPercentNonWorkingHours_and_getLimitByTurnoverPercentNonWorkingHours)
{
    config->setLimitByTurnoverPercentNonWorkingHours(10.0f);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 10.0f, 0.0001f);

    config->setLimitByTurnoverPercentNonWorkingHours(50.0f);
    ASSERT_NEAR(config->getLimitByTurnoverPercentNonWorkingHours(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setStorageMonthLimit_and_getStorageMonthLimit)
{
    config->setStorageMonthLimit(1);
    ASSERT_EQ(config->getStorageMonthLimit(), 1);

    config->setStorageMonthLimit(2);
    ASSERT_EQ(config->getStorageMonthLimit(), 2);
}

TEST_F(Test_Config, Test_setHighlightGoodOperations_and_isHighlightGoodOperations)
{
    config->setHighlightGoodOperations(false);
    ASSERT_EQ(config->isHighlightGoodOperations(), false);

    config->setHighlightGoodOperations(true);
    ASSERT_EQ(config->isHighlightGoodOperations(), true);
}

TEST_F(Test_Config, Test_setHighlightGoodOperationsYield_and_getHighlightGoodOperationsYield)
{
    config->setHighlightGoodOperationsYield(10.0f);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(), 10.0f, 0.0001f);

    config->setHighlightGoodOperationsYield(50.0f);
    ASSERT_NEAR(config->getHighlightGoodOperationsYield(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setHighlightBadOperations_and_isHighlightBadOperations)
{
    config->setHighlightBadOperations(false);
    ASSERT_EQ(config->isHighlightBadOperations(), false);

    config->setHighlightBadOperations(true);
    ASSERT_EQ(config->isHighlightBadOperations(), true);
}

TEST_F(Test_Config, Test_setHighlightBadOperationsLose_and_getHighlightBadOperationsLose)
{
    config->setHighlightBadOperationsLose(10.0f);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(), 10.0f, 0.0001f);

    config->setHighlightBadOperationsLose(50.0f);
    ASSERT_NEAR(config->getHighlightBadOperationsLose(), 50.0f, 0.0001f);
}

TEST_F(Test_Config, Test_setSimulatorConfigCommon_and_isSimulatorConfigCommon)
{
    config->setSimulatorConfigCommon(false);
    ASSERT_EQ(config->isSimulatorConfigCommon(), false);

    config->setSimulatorConfigCommon(true);
    ASSERT_EQ(config->isSimulatorConfigCommon(), true);
}

TEST_F(Test_Config, Test_setAutoPilotConfigCommon_and_isAutoPilotConfigCommon)
{
    config->setAutoPilotConfigCommon(false);
    ASSERT_EQ(config->isAutoPilotConfigCommon(), false);

    config->setAutoPilotConfigCommon(true);
    ASSERT_EQ(config->isAutoPilotConfigCommon(), true);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
