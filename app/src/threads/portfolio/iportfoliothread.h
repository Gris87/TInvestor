#pragma once



#include <QThread>



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
};
