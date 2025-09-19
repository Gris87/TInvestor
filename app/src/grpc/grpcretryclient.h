#pragma once



#include "src/grpc/igrpcretryclient.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class GrpcRetryClient : public IGrpcRetryClient
{
public:
    explicit GrpcRetryClient(IUserStorage* userStorage, IGrpcClient* grpcClient, ITimeUtils* timeUtils);
    ~GrpcRetryClient() override;

    GrpcRetryClient(const GrpcRetryClient& another)            = delete;
    GrpcRetryClient& operator=(const GrpcRetryClient& another) = delete;

    std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
    getValidOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor) override;
    std::shared_ptr<tinkoff::PortfolioResponse> getValidPortfolio(QThread* parentThread, const QString& accountId) override;

private:
    bool validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    IUserStorage* mUserStorage;
    IGrpcClient*  mGrpcClient;
    ITimeUtils*   mTimeUtils;
};
