#pragma once



#include "src/threads/bidirtrading/ibidirtradingthread.h"

#include <QReadWriteLock>

#include "src/config/iconfig.h"
#include "src/domain/stock/stock.h"
#include "src/domain/trading/bidirtradinginfo.h"
#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/utils/timeutils/itimeutils.h"
#include "src/utils/tradeutils/itradeutils.h"



class BiDirTradingThread : public IBiDirTradingThread
{
    Q_OBJECT

public:
    explicit BiDirTradingThread(
        IInstrumentsStorage* instrumentsStorage,
        IUserStorage*        userStorage,
        IConfig*             config,
        ITimeUtils*          timeUtils,
        ITradeUtils*         tradeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        Stock*               stock,
        BiDirMode            bidirMode,
        const QString&       cause,
        QObject*             parent = nullptr
    );
    ~BiDirTradingThread() override;

    BiDirTradingThread(const BiDirTradingThread& another)            = delete;
    BiDirTradingThread& operator=(const BiDirTradingThread& another) = delete;

    void run() override;

    void setMode(BiDirMode bidirMode, const QString& cause) override;

    [[nodiscard]]
    BiDirMode bidirMode() const;

    void terminateTrading() override;
    void terminateThread() override;

    bool trade();
    void getInstrumentData();

    void checkIfNeedToCancelAndCreateOrder(
        const std::shared_ptr<tinkoff::OrderState>& tinkoffOrder,
        qint64                                      amountOfLots,
        const Quotation&                            price,
        bool&                                       needToCancel,
        bool&                                       needToOrder
    );
    void sellWithPrice(const Quotation& price);
    void buyWithPrice(qint64 amountOfLots, const Quotation& price);
    bool isNeedToSellAsap(qint64 timestamp, BiDirMode mode, float part, float yield, float commission);
    void removeOwnOrdersFromOrderBook(
        tinkoff::GetOrderBookResponse&              tinkoffOrderBook,
        const std::shared_ptr<tinkoff::OrderState>& tinkoffBuyOrder,
        const std::shared_ptr<tinkoff::OrderState>& tinkoffSellOrder
    );
    double calculateBidPrice(const tinkoff::GetOrderBookResponse& tinkoffOrderBook, BiDirMode mode, qint64 maxQuantity);
    double calculateAskPrice(
        const tinkoff::GetOrderBookResponse& tinkoffOrderBook,
        BiDirMode                            mode,
        double                               totalCost,
        double                               instrumentCost,
        double                               instrumentAvgPrice,
        float                                commission
    );

private:
    void calculateTotalCostAndInstrumentCost(
        const tinkoff::PortfolioResponse& tinkoffPortfolio,
        double&                           totalCost,
        double&                           instrumentCost,
        qint64&                           instrumentLots,
        double&                           instrumentAvgPrice
    );
    void calculateBuySellPriceAndLots(
        const tinkoff::GetOrderBookResponse& tinkoffOrderBook,
        const tinkoff::PortfolioResponse&    tinkoffPortfolio,
        float                                commission,
        qint64&                              lotsToBuy,
        qint64&                              lotsToSell,
        Quotation&                           buyPrice,
        Quotation&                           sellPrice
    );
    qint64                calculateLotsToKeep(BiDirMode mode, double totalCost, double bidPrice);
    IDecisionMakerConfig* chooseDecisionConfig();

    void cancelBuyOrder();
    void cancelSellOrder();

    QReadWriteLock*      mRwMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IUserStorage*        mUserStorage;
    IConfig*             mConfig;
    ITimeUtils*          mTimeUtils;
    ITradeUtils*         mTradeUtils;
    IGrpcClient*         mGrpcClient;
    IGrpcRetryClient*    mGrpcRetryClient;
    ILogsThread*         mLogsThread;
    QString              mAccountId;
    Stock*               mStock;
    BiDirMode            mBidirMode;
    bool                 mTerminateTrading;
    QString              mInstrumentId;
    qint32               mInstrumentLot;
    Quotation            mMinPriceIncrement;
    QString              mBuyOrderId;
    QString              mSellOrderId;
};
