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

struct ReadLogosInfo
{
    ReadLogosInfo(IFileFactory* _fileFactory, const QStringList& files) :
        fileFactory(_fileFactory)
    {
        instrumentIds.resizeForOverwrite(files.size());
        logos.resizeForOverwrite(files.size());
    }

    IFileFactory*   fileFactory;
    QStringList     instrumentIds;
    QList<QPixmap*> logos;
};

static void
readLogosForParallel(QThread* parentThread, int /*threadId*/, QList<QString>& files, int start, int end, void* additionalArgs)
{
    ReadLogosInfo* readLogosInfo = reinterpret_cast<ReadLogosInfo*>(additionalArgs);

    IFileFactory* fileFactory      = readLogosInfo->fileFactory;
    QString*      instrumentsArray = readLogosInfo->instrumentIds.data();
    QPixmap**     logosArray       = readLogosInfo->logos.data();

    QString logosPath = qApp->applicationDirPath() + "/data/instruments/logos/";

    QString* filesArray = files.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        QString  instrumentId = filesArray[i].remove(".png");
        QPixmap* logo         = new QPixmap();

        const std::shared_ptr<IFile> logoFile = fileFactory->newInstance(logosPath + filesArray[i]);

        if (logoFile->open(QIODevice::ReadOnly))
        {
            const QByteArray content = logoFile->readAll();
            logoFile->close();

            logo->loadFromData(content, "PNG");
        }

        instrumentsArray[i] = instrumentId;
        logosArray[i]       = logo;
    }
}

Logos LogosDatabase::readLogos()
{
    Logos res;

    QString logosPath = qApp->applicationDirPath() + "/data/instruments/logos/";

    const std::shared_ptr<IDir> dir   = mDirFactory->newInstance(logosPath);
    QStringList                 files = dir->entryList(QStringList() << "*.png");

    ReadLogosInfo readLogosInfo(mFileFactory, files);
    processInParallel(files, readLogosForParallel, &readLogosInfo);

    for (int i = 0; i < files.size(); ++i)
    {
        res[readLogosInfo.instrumentIds[i]] = readLogosInfo.logos[i];
    }

    return res;
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
