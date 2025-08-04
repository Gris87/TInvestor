#pragma once



#include "src/db/portfolio/iportfoliodatabase.h"

#include <gmock/gmock.h>



class PortfolioDatabaseMock : public IPortfolioDatabase
{
public:
    explicit PortfolioDatabaseMock() :
        IPortfolioDatabase()
    {
    }
    ~PortfolioDatabaseMock() override = default;

    PortfolioDatabaseMock(const PortfolioDatabaseMock& another)            = delete;
    PortfolioDatabaseMock& operator=(const PortfolioDatabaseMock& another) = delete;

    MOCK_METHOD(Portfolio, readPortfolio, (int partId), (override));
    MOCK_METHOD(void, writePortfolio, (const Portfolio& portfolio, int partId), (override));
    MOCK_METHOD(void, deletePortfolio, (int partId), (override));
};
