#pragma once



#include "src/grpc/igrpcretryclient.h"

#include <gmock/gmock.h>



class GrpcRetryClientMock : public IGrpcRetryClient
{
    Q_OBJECT

public:
    explicit GrpcRetryClientMock(QObject* parent = nullptr) :
        IGrpcRetryClient(parent)
    {
    }
    ~GrpcRetryClientMock() override = default;

    GrpcRetryClientMock(const GrpcRetryClientMock& another)            = delete;
    GrpcRetryClientMock& operator=(const GrpcRetryClientMock& another) = delete;

    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetOperationsByCursorResponse>,
        getValidOperations,
        (QThread * parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PortfolioResponse>,
        getValidPortfolio,
        (QThread * parentThread, const QString& accountId),
        (override)
    );
};
