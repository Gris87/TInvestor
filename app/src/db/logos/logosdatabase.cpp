#include "src/db/logos/logosdatabase.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



LogosDatabase::LogosDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    ILogosDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory)
{
    qDebug() << "Create LogosDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/instruments/logos");
    Q_ASSERT_X(ok, "LogosDatabase::LogosDatabase()", "Failed to create dir");
}

LogosDatabase::~LogosDatabase()
{
    qDebug() << "Destroy LogosDatabase";
}

struct PrepareLogosInfo
{
    explicit PrepareLogosInfo(const QStringList& files)
    {
        instrumentIds.resizeForOverwrite(files.size());
        logos.resizeForOverwrite(files.size());
    }

    QStringList  instrumentIds;
    QList<Logo*> logos;
};

static void
prepareLogosForParallel(QThread* parentThread, int /*threadId*/, QList<QString>& files, int start, int end, void* additionalArgs)
{
    PrepareLogosInfo* prepareLogosInfo = reinterpret_cast<PrepareLogosInfo*>(additionalArgs);

    QString* instrumentsArray = prepareLogosInfo->instrumentIds.data();
    Logo**   logosArray       = prepareLogosInfo->logos.data();

    QString* filesArray = files.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        instrumentsArray[i] = filesArray[i].remove(".png");
        logosArray[i]       = new Logo();
    }
}

Logos LogosDatabase::prepareLogos()
{
    Logos res;

    const QString logosPath = qApp->applicationDirPath() + "/data/instruments/logos/";

    const std::shared_ptr<IDir> dir   = mDirFactory->newInstance(logosPath);
    QStringList                 files = dir->entryList(QStringList() << "*.png");

    PrepareLogosInfo prepareLogosInfo(files);
    processInParallel(files, prepareLogosForParallel, &prepareLogosInfo);

    for (int i = 0; i < files.size(); ++i)
    {
        res[prepareLogosInfo.instrumentIds.at(i)] = prepareLogosInfo.logos.at(i);
    }

    return res;
}

void LogosDatabase::readLogo(const QString& instrumentId, QPixmap* logo)
{
    const std::shared_ptr<IFile> logoFile =
        mFileFactory->newInstance(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), instrumentId));

    if (logoFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = logoFile->readAll();
        logoFile->close();

        logo->loadFromData(content, "PNG");
    }
}

void LogosDatabase::writeLogo(const QString& instrumentId, QPixmap* logo)
{
    const std::shared_ptr<IFile> logoFile =
        mFileFactory->newInstance(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), instrumentId));

    const bool ok = logoFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "LogosDatabase::writeLogo()", "Failed to open file");

    logo->save(logoFile->getDevice(), "PNG");
    logoFile->close();
}
