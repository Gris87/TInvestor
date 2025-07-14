#pragma once



#include "src/db/portfolio/iportfoliodatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class PortfolioDatabase : public IPortfolioDatabase
{
public:
    explicit PortfolioDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~PortfolioDatabase() override;

    PortfolioDatabase(const PortfolioDatabase& another)            = delete;
    PortfolioDatabase& operator=(const PortfolioDatabase& another) = delete;

    Portfolio readPortfolio() override;
    void      writePortfolio(const Portfolio& portfolio) override;

private:
    IFileFactory* mFileFactory;
};
