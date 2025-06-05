#pragma once



#include <QThread>



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

    virtual void setAccounts(const QString& account, const QString& anotherAccount) = 0;
    virtual void terminateThread()                                                  = 0;

signals:
    void accountNotFound();
};
