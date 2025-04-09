#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000;
constexpr qint32 NANOS_IN_MS  = 1000000;
constexpr float  NANOS_FLOAT  = 1000000000.0f;



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
