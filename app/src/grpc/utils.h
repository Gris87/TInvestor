#pragma once



#include "src/grpc/igrpcclient.h"



qint64 timeToTimestamp(const google::protobuf::Timestamp& timestamp);
float  quotationToFloat(const tinkoff::Quotation& quotation);
