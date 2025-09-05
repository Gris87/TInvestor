#pragma once



#include "src/threads/follow/ifollowthread.h"

#include <QReadWriteLock>

#include "src/domain/portfolio/portfoliominitem.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"



class FollowThread : public IFollowThread
{
    Q_OBJECT

public:
    explicit FollowThread(IInstrumentsStorage* instrumentsStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~FollowThread() override;

    FollowThread(const FollowThread& another)            = delete;
    FollowThread& operator=(const FollowThread& another) = delete;

    void run() override;

    void setAccounts(const QString& accountId, const QString& anotherAccountId, const QString& anotherAccountName) override;

    void terminateThread() override;

    bool createPortfolioStream();

private:
    void handlePortfolios(
        const std::shared_ptr<tinkoff::PortfolioResponse>& portfolio,
        const std::shared_ptr<tinkoff::PortfolioResponse>& anotherPortfolio
    );
    PortfolioMinItems buildInstrumentToCostMap(const std::shared_ptr<tinkoff::PortfolioResponse>& tinkoffPortfolio);
    double            calculateTotalCost(const PortfolioMinItems& instruments);
    void              buildInstrumentsForTrading(
                     const PortfolioMinItems& instruments,
                     const PortfolioMinItems& anotherInstruments,
                     double                   totalCost,
                     double                   anotherTotalCost,
                     InstrumentsForTrading&   instrumentsForSale,
                     InstrumentsForTrading&   instrumentsForBuy
                 );

    [[nodiscard]]
    qint32 getInstrumentLot(const QString& instrumentId) const;

    QReadWriteLock*                  mRwMutex;
    IInstrumentsStorage*             mInstrumentsStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    QString                          mAnotherAccountId;
    QString                          mAnotherAccountName;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
};
