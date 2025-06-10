#pragma once



#include <QThread>

#include <QMap>

#include "src/domain/trading/tradinginfo.h"



class IFollowThread : public QThread
{
    Q_OBJECT

public:
    explicit IFollowThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IFollowThread() override = default;

    IFollowThread(const IFollowThread& another)            = delete;
    IFollowThread& operator=(const IFollowThread& another) = delete;

    virtual void setAccounts(const QString& accountId, const QString& anotherAccountId, const QString& anotherAccountName) = 0;
    virtual void terminateThread()                                                                                         = 0;

signals:
    void tradeInstruments(const QMap<QString, TradingInfo>& instruments); // Instrument UID => TradingInfo
};
