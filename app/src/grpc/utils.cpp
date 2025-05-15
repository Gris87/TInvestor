#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND    = 1000LL;
constexpr qint32 NANOS_IN_MS     = 1000000;
constexpr qint32 NANOS_INT       = 1000000000;
constexpr float  NANOS_FLOAT     = 1000000000.0f;
constexpr qint8  START_PRECISION = 9;
constexpr qint8  TENS            = 10;



qint64 timeToTimestamp(const google::protobuf::Timestamp& timestamp)
{
    return (timestamp.seconds() * MS_IN_SECOND) + (timestamp.nanos() / NANOS_IN_MS);
}

static float unitsAndNanoToFloat(qint64 units, qint32 nano)
{
    return units + (nano / NANOS_FLOAT);
}

float quotationToFloat(const tinkoff::Quotation& quotation)
{
    return unitsAndNanoToFloat(quotation.units(), quotation.nano());
}

float quotationToFloat(const Quotation& quotation)
{
    return unitsAndNanoToFloat(quotation.units, quotation.nano);
}

float moneyToFloat(const tinkoff::MoneyValue& money)
{
    return unitsAndNanoToFloat(money.units(), money.nano());
}

static qint8 nanoPrecision(qint32 nano)
{
    qint8 res = START_PRECISION;

    while (res > 2)
    {
        if (nano % TENS != 0)
        {
            break;
        }

        nano /= TENS;
        --res;
    }

    return res;
}

qint8 quotationPrecision(const tinkoff::Quotation& quotation)
{
    return nanoPrecision(quotation.nano());
}

qint8 quotationPrecision(const Quotation& quotation)
{
    return nanoPrecision(quotation.nano);
}

qint8 moneyPrecision(const tinkoff::MoneyValue& money)
{
    return nanoPrecision(money.nano());
}

Quotation unitsAndNanoSum(qint64 units, qint32 nano, qint64 units2, qint32 nano2)
{
    Quotation res;

    res.nano = nano + nano2;

    res.units  = units + units2 + nano2 / NANOS_INT;
    res.nano  %= NANOS_INT;

    return res;
}

Quotation quotationSum(const Quotation& quotation1, const tinkoff::Quotation& quotation2)
{
    return unitsAndNanoSum(quotation1.units, quotation1.nano, quotation2.units(), quotation2.nano());
}

Quotation quotationSum(const Quotation& quotation1, const Quotation& quotation2)
{
    return unitsAndNanoSum(quotation1.units, quotation1.nano, quotation2.units, quotation2.nano);
}

Quotation quotationSum(const Quotation& quotation1, const tinkoff::MoneyValue& money)
{
    return unitsAndNanoSum(quotation1.units, quotation1.nano, money.units(), money.nano());
}
