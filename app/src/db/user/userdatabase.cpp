#include "src/db/user/userdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QHostInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>



constexpr quint64 CRYPT_KEY = 0x5EBEB228C0D4D48FULL;



UserDatabase::UserDatabase() :
    IUserDatabase(),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    mSimpleCrypt()
{
    qDebug() << "Create UserDatabase";

    const QString appDir = qApp->applicationDirPath();

    bool ok = QDir().mkpath(appDir + "/data/user");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const QString dbPath = appDir + "/data/user/user.db";
    db.setDatabaseName(dbPath);

    ok = db.open();
    Q_ASSERT_X(ok, __FUNCTION__, db.lastError().text().toLocal8Bit().constData());

    qInfo() << "User database" << dbPath << "created";

    initSimpleCrypt();
    createUserTable();
    createAccountsTable();
}

UserDatabase::~UserDatabase()
{
    qDebug() << "Destroy UserDatabase";

    db.close();
}

void UserDatabase::initSimpleCrypt()
{
    quint64   key      = CRYPT_KEY;
    quint8*   keyBytes = reinterpret_cast<quint8*>(&key);
    const int keySize  = sizeof(key);

#ifdef Q_OS_WINDOWS
    const QString user = qgetenv("USERNAME");
#else
    const QString user = qgetenv("USER");
#endif
    const QString host = QHostInfo::localHostName();

    if (user != "")
    {
        const QByteArray bytes = user.toUtf8();

        for (int i = 0; i < keySize; ++i)
        {
            keyBytes[i] ^= static_cast<quint8>(bytes.at(i % bytes.size()));
        }
    }

    if (host != "")
    {
        const QByteArray bytes = host.toUtf8();

        for (int i = 0; i < keySize; ++i)
        {
            keyBytes[keySize - i - 1] ^= static_cast<quint8>(bytes.at(i % bytes.size()));
        }
    }

    mSimpleCrypt.setKey(key);
}

void UserDatabase::createUserTable() const
{
    qDebug() << "Create table with user info";

    const QString str =
        "CREATE TABLE IF NOT EXISTS user ("
        "    id                      INTEGER NOT NULL PRIMARY KEY, "
        "    token                   TEXT NOT NULL, "
        "    qualified               BOOLEAN NOT NULL CHECK (qualified IN (0, 1)), "
        "    qualified_for_work_with TEXT NOT NULL, "
        "    tariff                  TEXT NOT NULL"
        ");";

    QSqlQuery query(db);

    const bool ok = query.exec(str);
    Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());
}

void UserDatabase::createAccountsTable() const
{
    qDebug() << "Create table with accounts";

    const QString str =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "    id   INTEGER NOT NULL PRIMARY KEY, "
        "    uid  TEXT NOT NULL, "
        "    name TEXT NOT NULL"
        ");";

    QSqlQuery query(db);

    const bool ok = query.exec(str);
    Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());
}

User UserDatabase::readUserInfo()
{
    qDebug() << "Reading user info";

    User res;

    const QString str =
        "SELECT token, qualified, qualified_for_work_with, tariff "
        "FROM user "
        "WHERE id = 1;";

    QSqlQuery query(db);

    const bool ok = query.exec(str);
    Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());

    const QSqlRecord rec = query.record();

    const int tokenIndex                = rec.indexOf("token");
    const int qualifiedIndex            = rec.indexOf("qualified");
    const int qualifiedForWorkWithIndex = rec.indexOf("qualified_for_work_with");
    const int tariffIndex               = rec.indexOf("tariff");

    if (query.first())
    {
        res.token                = mSimpleCrypt.decryptToString(query.value(tokenIndex).toString());
        res.qualified            = query.value(qualifiedIndex).toLongLong() != 0;
        res.qualifiedForWorkWith = query.value(qualifiedForWorkWithIndex).toString().split(',');
        res.setTariff(query.value(tariffIndex).toString());

        res.qualifiedForWorkWith.removeAll("");
    }
    else
    {
        res.token                = "";
        res.qualified            = false;
        res.qualifiedForWorkWith = QStringList();
        res.setTariff("fees");

        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO user "
            "(id, token, qualified, qualified_for_work_with, tariff) "
            "VALUES "
            "(:id, :token, :qualified, :qualified_for_work_with, :tariff);"
        );
        query.bindValue(":id", 1);
        query.bindValue(":token", mSimpleCrypt.encryptToString(res.token));
        query.bindValue(":qualified", res.qualified ? 1 : 0);
        query.bindValue(":qualified_for_work_with", res.qualifiedForWorkWith.isEmpty() ? "" : res.qualifiedForWorkWith.join(','));
        query.bindValue(":tariff", res.tariff);

        const bool ok = query.exec();
        Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());
    }

    return res;
}

Accounts UserDatabase::readAccounts()
{
    qDebug() << "Reading accounts";

    Accounts res;

    const QString str =
        "SELECT id, uid, name "
        "FROM accounts;";

    QSqlQuery query(db);

    const bool ok = query.exec(str);
    Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());

    const QSqlRecord rec = query.record();

    const int idIndex   = rec.indexOf("id");
    const int uidIndex  = rec.indexOf("uid");
    const int nameIndex = rec.indexOf("name");

    while (query.next())
    {
        Account account;

        account.index = query.value(idIndex).toInt() - 1;
        account.id    = mSimpleCrypt.decryptToString(query.value(uidIndex).toString());
        account.name  = query.value(nameIndex).toString();

        res[account.hash()] = account;
    }

    return res;
}

void UserDatabase::writeToken(const QString& token)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE user "
        "SET token = :token "
        "WHERE id = 1;"
    );
    query.bindValue(":token", mSimpleCrypt.encryptToString(token));

    const bool ok = query.exec();
    Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());
}

void UserDatabase::writeUserInfo(const User& user)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE user "
        "SET "
        "qualified = :qualified, "
        "qualified_for_work_with = :qualified_for_work_with, "
        "tariff = :tariff "
        "WHERE id = 1;"
    );
    query.bindValue(":qualified", user.qualified ? 1 : 0);
    query.bindValue(":qualified_for_work_with", user.qualifiedForWorkWith.isEmpty() ? "" : user.qualifiedForWorkWith.join(','));
    query.bindValue(":tariff", user.tariff);

    const bool ok = query.exec();
    Q_ASSERT_X(ok, __FUNCTION__, query.lastError().text().toLocal8Bit().constData());
}

void UserDatabase::writeAccounts(const Accounts& accounts)
{
    bool ok = db.transaction();
    Q_ASSERT_X(ok, __FUNCTION__, db.lastError().text().toLocal8Bit().constData());

    const QString str =
        "DELETE "
        "FROM accounts;";

    QSqlQuery query1(db);

    ok = query1.exec(str);
    Q_ASSERT_X(ok, __FUNCTION__, query1.lastError().text().toLocal8Bit().constData());

    QVariantList ids;
    QVariantList uids;
    QVariantList names;

    ids.resizeForOverwrite(accounts.size());
    uids.resizeForOverwrite(accounts.size());
    names.resizeForOverwrite(accounts.size());

    int i = 0;

    for (auto it = accounts.constBegin(), end = accounts.constEnd(); it != end; ++it)
    {
        ids[i]   = it.value().index + 1;
        uids[i]  = mSimpleCrypt.encryptToString(it.value().id);
        names[i] = it.value().name;

        ++i;
    }

    QSqlQuery query2(db);

    query2.prepare(
        "INSERT INTO accounts "
        "(id, uid, name) "
        "VALUES "
        "(:id, :uid, :name);"
    );
    query2.bindValue(":id", ids);
    query2.bindValue(":uid", uids);
    query2.bindValue(":name", names);

    ok = query2.execBatch();
    Q_ASSERT_X(ok, __FUNCTION__, query2.lastError().text().toLocal8Bit().constData());

    ok = db.commit();
    Q_ASSERT_X(ok, __FUNCTION__, db.lastError().text().toLocal8Bit().constData());
}
