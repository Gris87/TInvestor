#pragma once



#include "src/threads/portfolio/iportfoliothread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"



class PortfolioThread : public IPortfolioThread
{
    Q_OBJECT

public:
    explicit PortfolioThread(
        IInstrumentsStorage* instrumentsStorage, ILogosStorage* logosStorage, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~PortfolioThread() override;

    PortfolioThread(const PortfolioThread& another)            = delete;
    PortfolioThread& operator=(const PortfolioThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;
    void terminateThread() override;

    void createPortfolioStream();

private:
    void handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    IInstrumentsStorage*             mInstrumentsStorage;
    ILogosStorage*                   mLogosStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
    QStringList                      mSortedCategories;
    QMap<QString, QString>           mCategoryNames; // Category => Localized name
};
