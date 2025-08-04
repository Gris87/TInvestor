#include "src/db/portfolio/portfoliodatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



PortfolioDatabase::PortfolioDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage) :
    IPortfolioDatabase(),
    mFileFactory(fileFactory),
    mLogosStorage(logosStorage)
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

Portfolio PortfolioDatabase::readPortfolio(int partId)
{
    qDebug() << "Reading portfolio from database";

    Portfolio res;

    const std::shared_ptr<IFile> portfolioFile =
        mFileFactory->newInstance(QString("%1/data/simulator/%2").arg(qApp->applicationDirPath(), fileName(partId)));

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

        mLogosStorage->readLock();

        for (PortfolioCategoryItem& category : res.positions)
        {
            for (PortfolioItem& item : category.items)
            {
                item.instrumentLogo = mLogosStorage->getLogo(item.instrumentId);
            }
        }

        mLogosStorage->readUnlock();
    }

    return res;
}

void PortfolioDatabase::writePortfolio(const Portfolio& portfolio, int partId)
{
    qDebug() << "Writing portfolio to database";

    const QJsonDocument jsonDoc(portfolio.toJsonArray());

    const std::shared_ptr<IFile> portfolioFile =
        mFileFactory->newInstance(QString("%1/data/simulator/%2").arg(qApp->applicationDirPath(), fileName(partId)));

    const bool ok = portfolioFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    portfolioFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    portfolioFile->close();
}

void PortfolioDatabase::deletePortfolio(int partId)
{
    qDebug() << "Deleting portfolio";

    const std::shared_ptr<IFile> portfolioFile =
        mFileFactory->newInstance(QString("%1/data/simulator/%2").arg(qApp->applicationDirPath(), fileName(partId)));

    portfolioFile->remove();
}

QString PortfolioDatabase::fileName(int partId) const
{
    return partId < 0 ? "portfolio.json" : QString("portfolio%1.json").arg(partId);
}
