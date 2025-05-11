#include "src/db/instruments/instrumentsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



InstrumentsDatabase::InstrumentsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    IInstrumentsDatabase(),
    mFileFactory(fileFactory)
{
    qDebug() << "Create InstrumentsDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/instruments/logos");
    Q_ASSERT_X(ok, "InstrumentsDatabase::InstrumentsDatabase()", "Failed to create dir");
}

InstrumentsDatabase::~InstrumentsDatabase()
{
    qDebug() << "Destroy InstrumentsDatabase";
}

Instruments InstrumentsDatabase::readInstruments()
{
    qDebug() << "Reading instruments from database";

    Instruments res;

    const std::shared_ptr<IFile> instrumentsFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/instruments/instruments.json");

    if (instrumentsFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = instrumentsFile->readAll();
        instrumentsFile->close();

        QJsonParseError     parseError;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(content, &parseError);

        if (parseError.error == QJsonParseError::NoError)
        {
            const QJsonObject jsonInstruments = jsonDoc.object();

            for (auto i = jsonInstruments.constBegin(), end = jsonInstruments.constEnd(); i != end; ++i)
            {
                InstrumentInfo info;

                info.fromJsonObject(i.value().toObject());

                res[i.key()] = info;
            }
        }
    }

    return res;
}

void InstrumentsDatabase::writeInstruments(const Instruments& instruments)
{
    qDebug() << "Writing instruments to database";

    QJsonObject jsonInstruments;

    for (auto it = instruments.cbegin(), end = instruments.cend(); it != end; ++it)
    {
        jsonInstruments.insert(it.key(), it.value().toJsonObject());
    }

    const QJsonDocument jsonDoc(jsonInstruments);

    const std::shared_ptr<IFile> instrumentsFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/instruments/instruments.json");

    const bool ok = instrumentsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "InstrumentsDatabase::writeInstruments()", "Failed to open file");

    instrumentsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    instrumentsFile->close();
}
