#include "src/db/portfolio/portfoliodatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



PortfolioDatabase::PortfolioDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    IPortfolioDatabase(),
    mFileFactory(fileFactory)
{
    qDebug() << "Create PortfolioDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/simulator");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");
}

PortfolioDatabase::~PortfolioDatabase()
{
    qDebug() << "Destroy PortfolioDatabase";
}

Portfolio PortfolioDatabase::readPortfolio()
{
    qDebug() << "Reading portfolio from database";

    Portfolio res;

    const std::shared_ptr<IFile> portfolioFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/simulator/portfolio.json");

    if (portfolioFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = portfolioFile->readAll();
        portfolioFile->close();

        const simdjson::padded_string jsonData(content.toStdString());

        simdjson::ondemand::parser parser;

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);
            res.fromJsonArray(doc.get_array());
        }
        catch (...)
        {
            qWarning() << "Failed to parse portfolio";
        }
    }

    return res;
}

void PortfolioDatabase::writePortfolio(const Portfolio& portfolio)
{
    qDebug() << "Writing portfolio to database";

    const QJsonDocument jsonDoc(portfolio.toJsonArray());

    const std::shared_ptr<IFile> portfolioFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/simulator/portfolio.json");

    const bool ok = portfolioFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    portfolioFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    portfolioFile->close();
}
