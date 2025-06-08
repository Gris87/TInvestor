#pragma once



#include <QThread>

#include <QMap>



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

    virtual void setAccountIds(const QString& accountId, const QString& anotherAccountId) = 0;
    virtual void terminateThread()                                                        = 0;

signals:
    void tradeInstruments(const QMap<QString, double>& instruments); // Instrument UID => Expected cost
};
