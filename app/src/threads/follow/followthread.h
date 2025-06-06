#pragma once



#include "src/threads/follow/ifollowthread.h"

#include "src/domain/portfolio/portfoliominitem.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"



class FollowThread : public IFollowThread
{
    Q_OBJECT

public:
    explicit FollowThread(
        IUserStorage* userStorage, IInstrumentsStorage* instrumentsStorage, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~FollowThread() override;

    FollowThread(const FollowThread& another)            = delete;
    FollowThread& operator=(const FollowThread& another) = delete;

    void run() override;

    void setAccounts(const QString& account, const QString& anotherAccount) override;
    void terminateThread() override;

    void createPortfolioStream();

private:
    void handlePortfolios(
        const std::shared_ptr<tinkoff::PortfolioResponse>& portfolio,
        const std::shared_ptr<tinkoff::PortfolioResponse>& anotherPortfolio
    );
    PortfolioMinItems     buildInstrumentToCostMap(const std::shared_ptr<tinkoff::PortfolioResponse>& tinkoffPortfolio);
    double                calculateTotalCost(const PortfolioMinItems& instruments);
    QMap<QString, double> buildInstrumentsForSaleMap(
        const PortfolioMinItems& portfolio, const PortfolioMinItems& anotherPortfolio, double totalCost, double anotherTotalCost
    );
    QMap<QString, double> buildInstrumentsForBuyMap(
        const PortfolioMinItems& portfolio, const PortfolioMinItems& anotherPortfolio, double totalCost, double anotherTotalCost
    );

    IUserStorage*                    mUserStorage;
    IInstrumentsStorage*             mInstrumentsStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    QString                          mAnotherAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
};
