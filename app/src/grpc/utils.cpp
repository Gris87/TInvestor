#include "src/grpc/utils.h"



float quotationToFloat(const tinkoff::Quotation& quotation)
{
    return quotation.units() + quotation.nano() / 1000000000.0f;
}
