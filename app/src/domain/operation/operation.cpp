#include "src/domain/operation/operation.h"

#include <QJsonArray>

#include "src/utils/exception/exception.h"



constexpr float FLOAT_EPSILON = 0.0001f;



Operation::Operation() :
    timestamp(),
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    description(),
    price(),
    fifoItems(),
    avgPriceFifo(),
    avgPriceWavg(),
    quantity(),
    remainedQuantity(),
    payment(),
    avgCostFifo(),
    costFifo(),
    costWavg(),
    commission(),
    yield(),
    yieldWithCommission(),
    yieldWithCommissionPercent(),
    inputMoney(),
    maxInputMoney(),
    totalYieldWithCommission(),
    totalYieldWithCommissionPercent(),
    remainedMoney(),
    totalMoney(),
    pricePrecision(),
    paymentPrecision(),
    commissionPrecision()
{
}

static void operationTimestampParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->timestamp = value.get_int64();
}

static void operationInstrumentIdParse(Operation* operation, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    operation->instrumentId         = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void operationInstrumentTickerParse(Operation* operation, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    operation->instrumentTicker     = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void operationInstrumentNameParse(Operation* operation, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    operation->instrumentName       = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void operationDescriptionParse(Operation* operation, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    operation->description          = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void operationPriceParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->price = value.get_double();
}

static void operationFifoItemsParse(Operation* operation, simdjson::ondemand::value value)
{
    simdjson::ondemand::array jsonArray = value.get_array();

    operation->fifoItems.resizeForOverwrite(jsonArray.count_elements());
    int i = 0;

    for (const simdjson::ondemand::object jsonObject : jsonArray)
    {
        operation->fifoItems[i].fromJsonObject(jsonObject);
        ++i;
    }
}

static void operationAvgPriceFifoParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->avgPriceFifo = value.get_double();
}

static void operationAvgPriceWavgParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->avgPriceWavg = value.get_double();
}

static void operationQuantityParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->quantity = value.get_int64();
}

static void operationRemainedQuantityParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->remainedQuantity = value.get_int64();
}

static void operationPaymentParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->payment = value.get_double();
}

static void operationAvgCostFifoParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->avgCostFifo = value.get_double();
}

static void operationCostFifoParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->costFifo.fromJsonObject(value.get_object());
}

static void operationCostWavgParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->costWavg.fromJsonObject(value.get_object());
}

static void operationCommissionParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->commission = value.get_double();
}

static void operationYieldParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->yield = value.get_double();
}

static void operationYieldWithCommissionParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->yieldWithCommission = value.get_double();
}

static void operationYieldWithCommissionPercentParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->yieldWithCommissionPercent = value.get_double();
}

static void operationInputMoneyParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->inputMoney.fromJsonObject(value.get_object());
}

static void operationMaxInputMoneyParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->maxInputMoney.fromJsonObject(value.get_object());
}

static void operationTotalYieldWithCommissionParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->totalYieldWithCommission.fromJsonObject(value.get_object());
}

static void operationTotalYieldWithCommissionPercentParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->totalYieldWithCommissionPercent = value.get_double();
}

static void operationRemainedMoneyParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->remainedMoney.fromJsonObject(value.get_object());
}

static void operationTotalMoneyParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->totalMoney.fromJsonObject(value.get_object());
}

static void operationPricePrecisionParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->pricePrecision = value.get_int64();
}

static void operationPaymentPrecisionParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->paymentPrecision = value.get_int64();
}

static void operationCommissionPrecisionParse(Operation* operation, simdjson::ondemand::value value)
{
    operation->commissionPrecision = value.get_int64();
}

static void operationThrowParseException(
    Operation* /*operation*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(Operation* operation, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"timestamp",                       operationTimestampParse                      },
    {"instrumentId",                    operationInstrumentIdParse                   },
    {"instrumentTicker",                operationInstrumentTickerParse               },
    {"instrumentName",                  operationInstrumentNameParse                 },
    {"description",                     operationDescriptionParse                    },
    {"price",                           operationPriceParse                          },
    {"fifoItems",                       operationFifoItemsParse                      },
    {"avgPriceFifo",                    operationAvgPriceFifoParse                   },
    {"avgPriceWavg",                    operationAvgPriceWavgParse                   },
    {"quantity",                        operationQuantityParse                       },
    {"remainedQuantity",                operationRemainedQuantityParse               },
    {"payment",                         operationPaymentParse                        },
    {"avgCostFifo",                     operationAvgCostFifoParse                    },
    {"costFifo",                        operationCostFifoParse                       },
    {"costWavg",                        operationCostWavgParse                       },
    {"commission",                      operationCommissionParse                     },
    {"yield",                           operationYieldParse                          },
    {"yieldWithCommission",             operationYieldWithCommissionParse            },
    {"yieldWithCommissionPercent",      operationYieldWithCommissionPercentParse     },
    {"inputMoney",                      operationInputMoneyParse                     },
    {"maxInputMoney",                   operationMaxInputMoneyParse                  },
    {"totalYieldWithCommission",        operationTotalYieldWithCommissionParse       },
    {"totalYieldWithCommissionPercent", operationTotalYieldWithCommissionPercentParse},
    {"remainedMoney",                   operationRemainedMoneyParse                  },
    {"totalMoney",                      operationTotalMoneyParse                     },
    {"pricePrecision",                  operationPricePrecisionParse                 },
    {"paymentPrecision",                operationPaymentPrecisionParse               },
    {"commissionPrecision",             operationCommissionPrecisionParse            }
};
// clang-format on

void Operation::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, operationThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    QJsonArray jsonFifoItems;

    for (const OperationFifoItem& fifoItem : fifoItems)
    {
        jsonFifoItems.append(fifoItem.toJsonObject());
    }

    // clang-format off
    res.insert("timestamp",                       timestamp);
    res.insert("instrumentId",                    instrumentId);
    res.insert("instrumentTicker",                instrumentTicker);
    res.insert("instrumentName",                  instrumentName);
    res.insert("description",                     description);
    res.insert("price",                           price);
    res.insert("fifoItems",                       jsonFifoItems);
    res.insert("avgPriceFifo",                    avgPriceFifo);
    res.insert("avgPriceWavg",                    avgPriceWavg);
    res.insert("quantity",                        quantity);
    res.insert("remainedQuantity",                remainedQuantity);
    res.insert("payment",                         payment);
    res.insert("avgCostFifo",                     avgCostFifo);
    res.insert("costFifo",                        costFifo.toJsonObject());
    res.insert("costWavg",                        costWavg.toJsonObject());
    res.insert("commission",                      commission);
    res.insert("yield",                           yield);
    res.insert("yieldWithCommission",             yieldWithCommission);
    res.insert("yieldWithCommissionPercent",      yieldWithCommissionPercent);
    res.insert("inputMoney",                      inputMoney.toJsonObject());
    res.insert("maxInputMoney",                   maxInputMoney.toJsonObject());
    res.insert("totalYieldWithCommission",        totalYieldWithCommission.toJsonObject());
    res.insert("totalYieldWithCommissionPercent", totalYieldWithCommissionPercent);
    res.insert("remainedMoney",                   remainedMoney.toJsonObject());
    res.insert("totalMoney",                      totalMoney.toJsonObject());
    res.insert("pricePrecision",                  pricePrecision);
    res.insert("paymentPrecision",                paymentPrecision);
    res.insert("commissionPrecision",             commissionPrecision);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId &&
           lhs.instrumentTicker == rhs.instrumentTicker && lhs.instrumentName == rhs.instrumentName &&
           lhs.description == rhs.description && qAbs(lhs.price - rhs.price) < FLOAT_EPSILON && lhs.fifoItems == rhs.fifoItems &&
           qAbs(lhs.avgPriceFifo - rhs.avgPriceFifo) < FLOAT_EPSILON &&
           qAbs(lhs.avgPriceWavg - rhs.avgPriceWavg) < FLOAT_EPSILON && lhs.quantity == rhs.quantity &&
           lhs.remainedQuantity == rhs.remainedQuantity && qAbs(lhs.payment - rhs.payment) < FLOAT_EPSILON &&
           qAbs(lhs.avgCostFifo - rhs.avgCostFifo) < FLOAT_EPSILON && lhs.costFifo == rhs.costFifo &&
           lhs.costWavg == rhs.costWavg && qAbs(lhs.commission - rhs.commission) < FLOAT_EPSILON &&
           qAbs(lhs.yield - rhs.yield) < FLOAT_EPSILON &&
           qAbs(lhs.yieldWithCommission - rhs.yieldWithCommission) < FLOAT_EPSILON &&
           qAbs(lhs.yieldWithCommissionPercent - rhs.yieldWithCommissionPercent) < FLOAT_EPSILON &&
           lhs.inputMoney == rhs.inputMoney && lhs.maxInputMoney == rhs.maxInputMoney &&
           lhs.totalYieldWithCommission == rhs.totalYieldWithCommission &&
           qAbs(lhs.totalYieldWithCommissionPercent - rhs.totalYieldWithCommissionPercent) < FLOAT_EPSILON &&
           lhs.remainedMoney == rhs.remainedMoney && lhs.totalMoney == rhs.totalMoney &&
           lhs.pricePrecision == rhs.pricePrecision && lhs.paymentPrecision == rhs.paymentPrecision &&
           lhs.commissionPrecision == rhs.commissionPrecision;
}
