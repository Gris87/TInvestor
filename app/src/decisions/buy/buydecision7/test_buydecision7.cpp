#include "src/decisions/buy/buydecision7/buydecision7.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision7config/ibuydecision7config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY      = 24LL * ONE_HOUR;



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision7 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        timeUtilsMock = new StrictMock<TimeUtilsMock>();

        buyDecision7 = new BuyDecision7(timeUtilsMock);
    }

    void TearDown() override
    {
        delete buyDecision7;
        delete timeUtilsMock;
    }

    void fillWithData(Stock* stock, QList<float> data)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            stockData.timestamp = 1704056400000 + i * ONE_MINUTE;
            stockData.price     = data.at(i);

            stock->data.append(stockData);
        }
    }

    BuyDecision7*              buyDecision7;
    StrictMock<TimeUtilsMock>* timeUtilsMock;
};



TEST_F(Test_BuyDecision7, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision7, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision7ConfigMock>  decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;
    stock.meta.lastTradeTime  = QTime(23, 50);

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: It's not the end of the day
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(false));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price without night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(0));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price with night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    stock.data[0].timestamp -= 2 * ONE_DAY;
    stock.data[2].timestamp  = stock.data[1].timestamp + 30 * ONE_MINUTE;

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price with night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    stock.data[0].timestamp -= ONE_DAY;
    stock.data[2].timestamp  = stock.data[1].timestamp + 30 * ONE_MINUTE;

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Price changed after night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
        }
    );

    stock.data[0].timestamp -= ONE_DAY;

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to buy because the price raises more than 2.00% during last 1 nights");

    // ====================================================================
    // TEST CASE: It's not the end of the day
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(false));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price without night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(0));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price with night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    stock.data[0].timestamp -= 2 * ONE_DAY;
    stock.data[2].timestamp  = stock.data[1].timestamp + 30 * ONE_MINUTE;

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price with night 2
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    stock.data[0].timestamp -= ONE_DAY;
    stock.data[2].timestamp  = stock.data[1].timestamp + 30 * ONE_MINUTE;

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Price changed after night
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
        }
    );

    stock.data[0].timestamp -= ONE_DAY;

    EXPECT_CALL(configMock, getBuyDecision7Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceRaise()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(1));
    EXPECT_CALL(*timeUtilsMock, isTimeBetween(_, QTime(23, 40), QTime(23, 50))).WillOnce(Return(true));

    cause = buyDecision7->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to buy because the price raises more than 2.00% during last 1 nights");
}

TEST_F(Test_BuyDecision7, Test_asapMode)
{
    ASSERT_EQ(buyDecision7->asapMode(), ASAP_MODE_IMMEDIATELY_TRADE);
}
