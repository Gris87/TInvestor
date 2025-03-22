#include "src/grpc/utils.h"



qint64 timeToTimestamp(const google::protobuf::Timestamp& timestamp)
{
    return timestamp.seconds() * 1000 + timestamp.nanos() / 1000000;
}

float quotationToFloat(const tinkoff::Quotation& quotation)
{
    return quotation.units() + quotation.nano() / 1000000000.0f;
}
