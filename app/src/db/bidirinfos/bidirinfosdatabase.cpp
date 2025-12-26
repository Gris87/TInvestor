#include "src/db/bidirinfos/bidirinfosdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



BidirInfosDatabase::BidirInfosDatabase(IDirFactory* dirFactory) :
    IBidirInfosDatabase()
{
    qDebug() << "Create BidirInfosDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/bidirinfo");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");
}

BidirInfosDatabase::~BidirInfosDatabase()
{
    qDebug() << "Destroy BidirInfosDatabase";
}

BidirInfos BidirInfosDatabase::readBidirInfos(std::shared_ptr<IFile> bidirInfoFile)
{
    qDebug() << "Reading bi-dir information from database";

    BidirInfos res;

    if (bidirInfoFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = bidirInfoFile->readAll();
        bidirInfoFile->close();

        const simdjson::padded_string jsonData(content.toStdString());

        simdjson::ondemand::parser parser;

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            simdjson::ondemand::object jsonBidirInfos = doc.get_object();

            for (simdjson::ondemand::field field : jsonBidirInfos)
            {
                const std::string_view fieldStr     = field.escaped_key();
                const QString          instrumentId = QString::fromUtf8(fieldStr.data(), fieldStr.size());

                res[instrumentId].fromJsonObject(field.value().get_object());
            }
        }
        catch (...)
        {
            qWarning() << "Failed to parse bi-dir information";
        }
    }

    return res;
}
