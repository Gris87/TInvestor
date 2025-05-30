#pragma once



#include "src/threads/portfoliolastprice/iportfoliolastpricethread.h"

#include "src/grpc/igrpcclient.h"
#include "src/utils/timeutils/itimeutils.h"



class PortfolioLastPriceThread : public IPortfolioLastPriceThread
{
    Q_OBJECT

public:
    explicit PortfolioLastPriceThread(ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~PortfolioLastPriceThread() override;

    PortfolioLastPriceThread(const PortfolioLastPriceThread& another)            = delete;
    PortfolioLastPriceThread& operator=(const PortfolioLastPriceThread& another) = delete;

    void run() override;

    void portfolioChanged(const Portfolio& portfolio) override;
    void terminateThread() override;

    void createMarketDataStream();

private:
    ITimeUtils*                       mTimeUtils;
    IGrpcClient*                      mGrpcClient;
    std::shared_ptr<MarketDataStream> mMarketDataStream;
    QStringList                       mInstrumentIds;
};
