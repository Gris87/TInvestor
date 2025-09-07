#pragma once



#include "src/threads/bidirtrading/ibidirtradingthread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/utils/timeutils/itimeutils.h"



class BiDirTradingThread : public IBiDirTradingThread
{
    Q_OBJECT

public:
    explicit BiDirTradingThread(
        IInstrumentsStorage* instrumentsStorage,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        const QString&       cause,
        QObject*             parent = nullptr
    );
    ~BiDirTradingThread() override;

    BiDirTradingThread(const BiDirTradingThread& another)            = delete;
    BiDirTradingThread& operator=(const BiDirTradingThread& another) = delete;

    void run() override;

    void terminateTrading() override;
    void terminateThread() override;

    bool trade();
    void getInstrumentData();

private:
    std::shared_ptr<tinkoff::PortfolioResponse> getValidPortfolio();
    bool                                        validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    void cancelBuyOrder();
    void cancelSellOrder();

    IInstrumentsStorage* mInstrumentsStorage;
    ITimeUtils*          mTimeUtils;
    IGrpcClient*         mGrpcClient;
    ILogsThread*         mLogsThread;
    QString              mAccountId;
    QString              mInstrumentId;
    bool                 mTerminateTrading;
    qint32               mInstrumentLot;
    Quotation            mMinPriceIncrement;
    QString              mBuyOrderId;
    QString              mSellOrderId;
    Quotation            mLastBuyOrderPrice;
    Quotation            mLastSellOrderPrice;
};
