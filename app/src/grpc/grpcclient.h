#pragma once



#include "src/grpc/igrpcclient.h"

#include "src/grpc/irawgrpcclient.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class GrpcClient : public IGrpcClient
{
    Q_OBJECT

public:
    explicit GrpcClient(
        IUserStorage* userStorage, IRawGrpcClient* rawGrpcClient, ITimeUtils* timeUtils, QObject* parent = nullptr
    );
    ~GrpcClient();

    GrpcClient(const GrpcClient& another)            = delete;
    GrpcClient& operator=(const GrpcClient& another) = delete;

    std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo(QThread* parentThread) override;
    std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts(QThread* parentThread) override;
    std::shared_ptr<tinkoff::SharesResponse>      findStocks(QThread* parentThread) override;
    std::shared_ptr<tinkoff::GetCandlesResponse>
                                      getCandles(QThread* parentThread, const QString& uid, qint64 from, qint64 to) override;
    std::shared_ptr<MarketDataStream> createMarketDataStream() override;
    bool subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& uids) override;
    bool unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    std::shared_ptr<tinkoff::MarketDataResponse>
         readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;

private:
    QMutex*                                                 mMutex;
    IRawGrpcClient*                                         mRawGrpcClient;
    ITimeUtils*                                             mTimeUtils;
    std::shared_ptr<grpc::CallCredentials>                  mCreds;
    std::unique_ptr<tinkoff::UsersService::Stub>            mUsersService;
    std::unique_ptr<tinkoff::InstrumentsService::Stub>      mInstrumentsService;
    std::unique_ptr<tinkoff::MarketDataService::Stub>       mMarketDataService;
    std::unique_ptr<tinkoff::MarketDataStreamService::Stub> mMarketDataStreamService;
};
