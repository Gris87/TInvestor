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
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");
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

        const simdjson::padded_string jsonData(content.toStdString());

        simdjson::ondemand::parser parser;

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            simdjson::ondemand::object jsonInstruments = doc.get_object();

            for (simdjson::ondemand::field field : jsonInstruments)
            {
                const std::string_view fieldStr     = field.escaped_key();
                const QString          instrumentId = QString::fromUtf8(fieldStr.data(), fieldStr.size());

                res[instrumentId].fromJsonObject(field.value().get_object());
            }
        }
        catch (...)
        {
            qWarning() << "Failed to parse instruments";
        }
    }

    return res;
}

void InstrumentsDatabase::writeInstruments(const Instruments& instruments)
{
    qDebug() << "Writing instruments to database";

    QJsonObject jsonInstruments;

    for (auto it = instruments.constBegin(), end = instruments.constEnd(); it != end; ++it)
    {
        jsonInstruments.insert(it.key(), it.value().toJsonObject());
    }

    const QJsonDocument jsonDoc(jsonInstruments);

    const std::shared_ptr<IFile> instrumentsFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/instruments/instruments.json");

    const bool ok = instrumentsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    instrumentsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    instrumentsFile->close();
}
