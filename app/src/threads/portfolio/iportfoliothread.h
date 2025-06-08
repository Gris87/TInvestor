#pragma once



#include <QThread>

#include "src/domain/portfolio/portfolio.h"



class IPortfolioThread : public QThread
{
    Q_OBJECT

public:
    explicit IPortfolioThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IPortfolioThread() override = default;

    IPortfolioThread(const IPortfolioThread& another)            = delete;
    IPortfolioThread& operator=(const IPortfolioThread& another) = delete;

    virtual void setAccountId(const QString& accountId) = 0;
    virtual void terminateThread()                      = 0;

signals:
    void portfolioChanged(const Portfolio& portfolio);
};
