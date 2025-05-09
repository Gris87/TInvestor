#pragma once



#include "src/threads/portfolio/iportfoliothread.h"

#include "src/storage/user/iuserstorage.h"



class PortfolioThread : public IPortfolioThread
{
    Q_OBJECT

public:
    explicit PortfolioThread(IUserStorage* userStorage, QObject* parent = nullptr);
    ~PortfolioThread() override;

    PortfolioThread(const PortfolioThread& another)            = delete;
    PortfolioThread& operator=(const PortfolioThread& another) = delete;

    void run() override;

private:
    IUserStorage* mUserStorage;
};
