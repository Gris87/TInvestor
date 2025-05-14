#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND    = 1000LL;
constexpr qint32 NANOS_IN_MS     = 1000000;
constexpr float  NANOS_FLOAT     = 1000000000.0f;
constexpr qint8  START_PRECISION = 9;
constexpr qint8  TENS            = 10;



qint64 timeToTimestamp(const google::protobuf::Timestamp& timestamp)
{
    return (timestamp.seconds() * MS_IN_SECOND) + (timestamp.nanos() / NANOS_IN_MS);
}

float quotationToFloat(const tinkoff::Quotation& quotation)
{
    return quotation.units() + (quotation.nano() / NANOS_FLOAT);
}

float quotationToFloat(const Quotation& quotation)
{
    return quotation.units + (quotation.nano / NANOS_FLOAT);
}

float moneyToFloat(const tinkoff::MoneyValue& money)
{
    return money.units() + (money.nano() / NANOS_FLOAT);
}

qint8 quotationPrecision(const tinkoff::Quotation& quotation)
{
    qint32 priceNanos = quotation.nano();

    qint8 res = START_PRECISION;

    while (res > 2)
    {
        if (priceNanos % TENS != 0)
        {
            break;
        }

        priceNanos /= TENS;
        --res;
    }

    return res;
}

qint8 quotationPrecision(const Quotation& quotation)
{
    qint32 priceNanos = quotation.nano;

    qint8 res = START_PRECISION;

    while (res > 2)
    {
        if (priceNanos % TENS != 0)
        {
            break;
        }

        priceNanos /= TENS;
        --res;
    }

    return res;
}

qint8 moneyPrecision(const tinkoff::MoneyValue& money)
{
    qint32 priceNanos = money.nano();

    qint8 res = START_PRECISION;

    while (res > 2)
    {
        if (priceNanos % TENS != 0)
        {
            break;
        }

        priceNanos /= TENS;
        --res;
    }

    return res;
}
