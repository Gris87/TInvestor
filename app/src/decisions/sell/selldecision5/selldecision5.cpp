#include "src/decisions/sell/selldecision5/selldecision5.h"

#include <QDebug>



SellDecision5::SellDecision5() :
    IActionDecision()
{
    qDebug() << "Create SellDecision5";
}

SellDecision5::~SellDecision5()
{
    qDebug() << "Destroy SellDecision5";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString SellDecision5::makeDecision(
    QThread*              parentThread,
    IDecisionMakerConfig* config,
    qint64                limitTimestamp,
    Stock*                stock,
    bool                  dateRange,
    int                   dataIndex,
    float                 price,
    float                 avgPrice,
    float                 commission
)
{
    Q_ASSERT_X(parentThread != nullptr, __FUNCTION__, "parentThread is invalid");
    Q_ASSERT_X(config != nullptr, __FUNCTION__, "config is invalid");
    Q_ASSERT_X(limitTimestamp >= 0, __FUNCTION__, "limitTimestamp is invalid");
    Q_ASSERT_X(stock != nullptr, __FUNCTION__, "stock is invalid");
    Q_ASSERT_X(
        (dateRange && dataIndex >= 0 && dataIndex < stock->data.size()) || (!dateRange && dataIndex == -1),
        __FUNCTION__,
        "dateRange and dataIndex are invalid"
    );
    Q_ASSERT_X(price > 0, __FUNCTION__, "price is invalid");
    Q_ASSERT_X(avgPrice > 0, __FUNCTION__, "avgPrice is invalid");
    Q_ASSERT_X(commission > 0, __FUNCTION__, "commission is invalid");

    ISellDecision5Config* sellConfig = config->getSellDecision5Config();

    if (sellConfig->isEnabled())
    {
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode SellDecision5::asapMode() const
{
    return ASAP_MODE_FOLLOW_PRICE;
}
