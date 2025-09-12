#pragma once



#include "src/grpc/igrpcretryclient.h"

#include "src/grpc/igrpcclient.h"
#include "src/utils/timeutils/itimeutils.h"



class GrpcRetryClient : public IGrpcRetryClient
{
    Q_OBJECT

public:
    explicit GrpcRetryClient(IGrpcClient* grpcClient, ITimeUtils* timeUtils, QObject* parent = nullptr);
    ~GrpcRetryClient() override;

    GrpcRetryClient(const GrpcRetryClient& another)            = delete;
    GrpcRetryClient& operator=(const GrpcRetryClient& another) = delete;

    std::shared_ptr<tinkoff::PortfolioResponse> getValidPortfolio(QThread* parentThread, const QString& accountId) override;
    std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
    getValidOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor) override;

private:
    IGrpcClient* mGrpcClient;
    ITimeUtils*  mTimeUtils;
};
