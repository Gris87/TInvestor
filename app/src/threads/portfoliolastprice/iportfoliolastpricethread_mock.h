#pragma once



#include "src/threads/portfoliolastprice/iportfoliolastpricethread.h"

#include <gmock/gmock.h>



class PortfolioLastPriceThreadMock : public IPortfolioLastPriceThread
{
    Q_OBJECT

public:
    explicit PortfolioLastPriceThreadMock(QObject* parent = nullptr) :
        IPortfolioLastPriceThread(parent)
    {
    }
    ~PortfolioLastPriceThreadMock() override = default;

    PortfolioLastPriceThreadMock(const PortfolioLastPriceThreadMock& another)            = delete;
    PortfolioLastPriceThreadMock& operator=(const PortfolioLastPriceThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, stocksChanged, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
