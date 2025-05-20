#pragma once



#include "src/domain/quotation/quotation.h"
#include "src/grpc/igrpcclient.h"



qint64    timeToTimestamp(const google::protobuf::Timestamp& timestamp);
float     quotationToFloat(const tinkoff::MoneyValue& money);
float     quotationToFloat(const tinkoff::Quotation& quotation);
float     quotationToFloat(const Quotation& quotation);
qint8     quotationPrecision(const tinkoff::MoneyValue& money);
qint8     quotationPrecision(const tinkoff::Quotation& quotation);
qint8     quotationPrecision(const Quotation& quotation);
Quotation quotationSum(const Quotation& quotation1, const tinkoff::MoneyValue& money);
Quotation quotationSum(const Quotation& quotation1, const tinkoff::Quotation& quotation2);
Quotation quotationSum(const tinkoff::MoneyValue& money, const Quotation& quotation2);
Quotation quotationSum(const tinkoff::Quotation& quotation1, const Quotation& quotation2);
Quotation quotationSum(const Quotation& quotation1, const Quotation& quotation2);
Quotation quotationDiff(const Quotation& quotation1, const tinkoff::MoneyValue& money);
Quotation quotationDiff(const Quotation& quotation1, const tinkoff::Quotation& quotation2);
Quotation quotationDiff(const tinkoff::MoneyValue& money, const Quotation& quotation2);
Quotation quotationDiff(const tinkoff::Quotation& quotation1, const Quotation& quotation2);
Quotation quotationDiff(const Quotation& quotation1, const Quotation& quotation2);
Quotation quotationMultiply(const tinkoff::MoneyValue& money, qint64 coef);
Quotation quotationMultiply(const tinkoff::Quotation& quotation, qint64 coef);
Quotation quotationMultiply(const Quotation& quotation, qint64 coef);
Quotation quotationDivide(const tinkoff::MoneyValue& money, qint64 coef);
Quotation quotationDivide(const tinkoff::Quotation& quotation, qint64 coef);
Quotation quotationDivide(const Quotation& quotation, qint64 coef);
