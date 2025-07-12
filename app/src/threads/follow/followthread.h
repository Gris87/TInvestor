#pragma once



#include "src/threads/follow/ifollowthread.h"

#include <QMutex>

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
    void setKeepMoney(int value) override;

    [[nodiscard]]
    int keepMoney() const;

    void terminateThread() override;

    void createPortfolioStream();

private:
    void handlePortfolios(
        const std::shared_ptr<tinkoff::PortfolioResponse>& portfolio,
        const std::shared_ptr<tinkoff::PortfolioResponse>& anotherPortfolio
    );
    PortfolioMinItems buildInstrumentToCostMap(const std::shared_ptr<tinkoff::PortfolioResponse>& tinkoffPortfolio);
    double            calculateTotalCost(const PortfolioMinItems& instruments);
    void              buildInstrumentsForTrading(
                     const PortfolioMinItems&    instruments,
                     const PortfolioMinItems&    anotherInstruments,
                     double                      totalCost,
                     double                      anotherTotalCost,
                     InstrumentsForTrading& instrumentsForSale,
                     InstrumentsForTrading& instrumentsForBuy
                 );

    QMutex*                          mMutex;
    IInstrumentsStorage*             mInstrumentsStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    QString                          mAnotherAccountId;
    QString                          mAnotherAccountName;
    int                              mKeepMoney;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
};
