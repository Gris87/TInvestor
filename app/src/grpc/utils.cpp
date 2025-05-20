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

float quotationToFloat(const tinkoff::MoneyValue& money)
{
    return unitsAndNanoToFloat(money.units(), money.nano());
}

float quotationToFloat(const tinkoff::Quotation& quotation)
{
    return unitsAndNanoToFloat(quotation.units(), quotation.nano());
}

float quotationToFloat(const Quotation& quotation)
{
    return unitsAndNanoToFloat(quotation.units, quotation.nano);
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

qint8 quotationPrecision(const tinkoff::MoneyValue& money)
{
    return nanoPrecision(money.nano());
}

qint8 quotationPrecision(const tinkoff::Quotation& quotation)
{
    return nanoPrecision(quotation.nano());
}

qint8 quotationPrecision(const Quotation& quotation)
{
    return nanoPrecision(quotation.nano);
}

static Quotation unitsAndNanoSum(qint64 units, qint64 nano, qint64 units2, qint64 nano2)
{
    Quotation res;

    const qint64 nano64 = nano + nano2;

    res.units = units + units2 + nano64 / NANOS_INT;
    res.nano  = nano64 % NANOS_INT;

    return res;
}

Quotation quotationSum(const Quotation& quotation1, const tinkoff::MoneyValue& money)
{
    return unitsAndNanoSum(quotation1.units, quotation1.nano, money.units(), money.nano());
}

Quotation quotationSum(const Quotation& quotation1, const tinkoff::Quotation& quotation2)
{
    return unitsAndNanoSum(quotation1.units, quotation1.nano, quotation2.units(), quotation2.nano());
}

Quotation quotationSum(const tinkoff::MoneyValue& money, const Quotation& quotation2)
{
    return unitsAndNanoSum(money.units(), money.nano(), quotation2.units, quotation2.nano);
}

Quotation quotationSum(const tinkoff::Quotation& quotation1, const Quotation& quotation2)
{
    return unitsAndNanoSum(quotation1.units(), quotation1.nano(), quotation2.units, quotation2.nano);
}

Quotation quotationSum(const Quotation& quotation1, const Quotation& quotation2)
{
    return unitsAndNanoSum(quotation1.units, quotation1.nano, quotation2.units, quotation2.nano);
}

static Quotation unitsAndNanoDiff(qint64 units, qint64 nano, qint64 units2, qint64 nano2)
{
    Quotation res;

    const qint64 nano64 = nano - nano2;

    res.units = units - units2 + nano64 / NANOS_INT;
    res.nano  = nano64 % NANOS_INT;

    return res;
}

Quotation quotationDiff(const Quotation& quotation1, const tinkoff::MoneyValue& money)
{
    return unitsAndNanoDiff(quotation1.units, quotation1.nano, money.units(), money.nano());
}

Quotation quotationDiff(const Quotation& quotation1, const tinkoff::Quotation& quotation2)
{
    return unitsAndNanoDiff(quotation1.units, quotation1.nano, quotation2.units(), quotation2.nano());
}

Quotation quotationDiff(const tinkoff::MoneyValue& money, const Quotation& quotation2)
{
    return unitsAndNanoDiff(money.units(), money.nano(), quotation2.units, quotation2.nano);
}

Quotation quotationDiff(const tinkoff::Quotation& quotation1, const Quotation& quotation2)
{
    return unitsAndNanoDiff(quotation1.units(), quotation1.nano(), quotation2.units, quotation2.nano);
}

Quotation quotationDiff(const Quotation& quotation1, const Quotation& quotation2)
{
    return unitsAndNanoDiff(quotation1.units, quotation1.nano, quotation2.units, quotation2.nano);
}

static Quotation unitsAndNanoMultiply(qint64 units, qint64 nano, qint64 coef)
{
    Quotation res;

    const qint64 nano64 = nano * coef;

    res.units = units * coef + nano64 / NANOS_INT;
    res.nano  = nano64 % NANOS_INT;

    return res;
}

Quotation quotationMultiply(const tinkoff::MoneyValue& money, qint64 coef)
{
    return unitsAndNanoMultiply(money.units(), money.nano(), coef);
}

Quotation quotationMultiply(const tinkoff::Quotation& quotation, qint64 coef)
{
    return unitsAndNanoMultiply(quotation.units(), quotation.nano(), coef);
}

Quotation quotationMultiply(const Quotation& quotation, qint64 coef)
{
    return unitsAndNanoMultiply(quotation.units, quotation.nano, coef);
}

static Quotation unitsAndNanoDivide(qint64 units, qint32 nano, qint64 coef)
{
    Quotation res;

    res.units = units / coef;
    res.nano  = static_cast<qint32>(static_cast<double>(units % coef) / coef * NANOS_INT) + nano / coef;

    return res;
}

Quotation quotationDivide(const tinkoff::MoneyValue& money, qint64 coef)
{
    return unitsAndNanoDivide(money.units(), money.nano(), coef);
}

Quotation quotationDivide(const tinkoff::Quotation& quotation, qint64 coef)
{
    return unitsAndNanoDivide(quotation.units(), quotation.nano(), coef);
}

Quotation quotationDivide(const Quotation& quotation, qint64 coef)
{
    return unitsAndNanoDivide(quotation.units, quotation.nano, coef);
}
