#include "src/decisions/buy/buydecision8/buydecision8.h"

#include <QDebug>



BuyDecision8::BuyDecision8() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision8";
}

BuyDecision8::~BuyDecision8()
{
    qDebug() << "Destroy BuyDecision8";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision8::makeDecision(
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

    IBuyDecision8Config* buyConfig = config->getBuyDecision8Config();

    if (buyConfig->isEnabled())
    {
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode BuyDecision8::asapMode() const
{
    return ASAP_MODE_NONE;
}
