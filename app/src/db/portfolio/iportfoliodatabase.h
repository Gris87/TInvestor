#pragma once



#include "src/domain/portfolio/portfolio.h"



class IPortfolioDatabase
{
public:
    explicit IPortfolioDatabase() = default;
    virtual ~IPortfolioDatabase() = default;

    IPortfolioDatabase(const IPortfolioDatabase& another)            = delete;
    IPortfolioDatabase& operator=(const IPortfolioDatabase& another) = delete;

    virtual Portfolio readPortfolio(int partId = -1)                              = 0;
    virtual void      writePortfolio(const Portfolio& portfolio, int partId = -1) = 0;
    virtual void      deletePortfolio(int partId = -1)                            = 0;
};
