#pragma once



#include "src/threads/portfolio/iportfoliothread.h"

#include <gmock/gmock.h>



class PortfolioThreadMock : public IPortfolioThread
{
    Q_OBJECT

public:
    explicit PortfolioThreadMock(QObject* parent = nullptr) :
        IPortfolioThread(parent)
    {
    }
    ~PortfolioThreadMock() override = default;

    PortfolioThreadMock(const PortfolioThreadMock& another)            = delete;
    PortfolioThreadMock& operator=(const PortfolioThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccount, (const QString& account), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
