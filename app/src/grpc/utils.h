#pragma once



#include "src/domain/quotation/quotation.h"
#include "src/grpc/igrpcclient.h"



qint64 timeToTimestamp(const google::protobuf::Timestamp& timestamp);
float  quotationToFloat(const tinkoff::Quotation& quotation);
float  quotationToFloat(const Quotation& quotation);
float  moneyToFloat(const tinkoff::MoneyValue& money);
