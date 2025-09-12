#pragma once



#include "src/threads/portfolio/iportfoliothread.h"

#include <QReadWriteLock>

#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class PortfolioThread : public IPortfolioThread
{
    Q_OBJECT

public:
    explicit PortfolioThread(
        IInstrumentsStorage* instrumentsStorage,
        ILogosStorage*       logosStorage,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        QObject*             parent = nullptr
    );
    ~PortfolioThread() override;

    PortfolioThread(const PortfolioThread& another)            = delete;
    PortfolioThread& operator=(const PortfolioThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;
    void terminateThread() override;

    bool createPortfolioStream();

private:
    bool requestPortfolio();
    void handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    QReadWriteLock*                  mRwMutex;
    IInstrumentsStorage*             mInstrumentsStorage;
    ILogosStorage*                   mLogosStorage;
    ITimeUtils*                      mTimeUtils;
    IGrpcClient*                     mGrpcClient;
    IGrpcRetryClient*                mGrpcRetryClient;
    QString                          mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
    QStringList                      mSortedCategories;
    QMap<QString, QString>           mCategoryNames; // Category => Localized name
};
