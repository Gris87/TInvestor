#pragma once



#include <QObject>

#include <QThread>

#include "src/grpc/irawgrpcclient.h"



class IGrpcRetryClient : public QObject
{
    Q_OBJECT

public:
    explicit IGrpcRetryClient(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    ~IGrpcRetryClient() override = default;

    IGrpcRetryClient(const IGrpcRetryClient& another)            = delete;
    IGrpcRetryClient& operator=(const IGrpcRetryClient& another) = delete;

    virtual std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
    getValidOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor)     = 0;
    virtual std::shared_ptr<tinkoff::PortfolioResponse> getValidPortfolio(QThread* parentThread, const QString& accountId) = 0;
};
