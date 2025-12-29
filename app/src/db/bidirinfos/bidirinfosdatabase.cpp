#include "src/db/bidirinfos/bidirinfosdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



BiDirInfosDatabase::BiDirInfosDatabase(IDirFactory* dirFactory) :
    IBiDirInfosDatabase()
{
    qDebug() << "Create BiDirInfosDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/bidirinfo");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");
}

BiDirInfosDatabase::~BiDirInfosDatabase()
{
    qDebug() << "Destroy BiDirInfosDatabase";
}

BiDirInfos BiDirInfosDatabase::readBiDirInfos(std::shared_ptr<IFile> biDirInfoFile)
{
    qDebug() << "Reading bi-dir information from database";

    BiDirInfos res;

    if (biDirInfoFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = biDirInfoFile->readAll();
        biDirInfoFile->close();

        const simdjson::padded_string jsonData(content.toStdString());

        simdjson::ondemand::parser parser;

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            simdjson::ondemand::object jsonBiDirInfos = doc.get_object();

            for (simdjson::ondemand::field field : jsonBiDirInfos)
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
