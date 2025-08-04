#pragma once



#include "src/db/portfolio/iportfoliodatabase.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class PortfolioDatabase : public IPortfolioDatabase
{
public:
    explicit PortfolioDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage);
    ~PortfolioDatabase() override;

    PortfolioDatabase(const PortfolioDatabase& another)            = delete;
    PortfolioDatabase& operator=(const PortfolioDatabase& another) = delete;

    Portfolio readPortfolio(int partId = -1) override;
    void      writePortfolio(const Portfolio& portfolio, int partId = -1) override;
    void      deletePortfolio(int partId = -1) override;

    [[nodiscard]]
    QString fileName(int partId) const;

private:
    IFileFactory*  mFileFactory;
    ILogosStorage* mLogosStorage;
};
