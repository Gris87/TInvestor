#include "src/decisions/buy/buydecision7/buydecision7.h"

#include <QDebug>



BuyDecision7::BuyDecision7() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision7";
}

BuyDecision7::~BuyDecision7()
{
    qDebug() << "Destroy BuyDecision7";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision7::makeDecision(
    QThread*              parentThread,
    IDecisionMakerConfig* config,
    qint64                limitTimestamp,
    Stock*                stock,
    bool                  dateRange,
    int                   dataIndex,
    float                 price,
    float /*avgPrice*/,
    float /*commission*/
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

    IBuyDecision7Config* buyConfig = config->getBuyDecision7Config();

    if (buyConfig->isEnabled())
    {
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode BuyDecision7::asapMode() const
{
    return ASAP_MODE_IMMEDIATELY_TRADE;
}
