#include "src/db/user/userdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>



#define CRYPT_KEY 0x5EBEB228C0D4D48F



UserDatabase::UserDatabase() :
    IUserDatabase(),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    mSimpleCrypt(CRYPT_KEY)
{
    qDebug() << "Create UserDatabase";

    QString appDir = qApp->applicationDirPath();

    bool ok = QDir().mkpath(appDir + "/data/user");
    Q_ASSERT_X(ok, "UserDatabase::UserDatabase()", "Failed to create dir");

    QString dbPath = appDir + "/data/user/user.db";
    db.setDatabaseName(dbPath);

    ok = db.open();
    Q_ASSERT_X(ok, "UserDatabase::UserDatabase()", db.lastError().text().toLocal8Bit().constData());

    qInfo() << "User database" << dbPath << "created";

    createUserTable();
    createAccountsTable();
}

UserDatabase::~UserDatabase()
{
    qDebug() << "Destroy UserDatabase";

    db.close();
}

void UserDatabase::createUserTable()
{
    qDebug() << "Create table with user info";

    QString str =
        "CREATE TABLE IF NOT EXISTS user ("
        "    id                      INTEGER NOT NULL PRIMARY KEY, "
        "    token                   TEXT NOT NULL, "
        "    qualified               BOOLEAN NOT NULL CHECK (qualified IN (0, 1)), "
        "    qualified_for_work_with TEXT NOT NULL, "
        "    tariff                  TEXT NOT NULL"
        ");";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::createUserTable()", query.lastError().text().toLocal8Bit().constData());
}

void UserDatabase::createAccountsTable()
{
    qDebug() << "Create table with accounts";

    QString str =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "    id   TEXT NOT NULL PRIMARY KEY, "
        "    name TEXT NOT NULL"
        ");";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::createAccountsTable()", query.lastError().text().toLocal8Bit().constData());
}

User UserDatabase::readUserInfo()
{
    qDebug() << "Reading user info";

    User res;

    QString str =
        "SELECT token, qualified, qualified_for_work_with, tariff "
        "FROM user "
        "WHERE id = 1;";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::readUserInfo()", query.lastError().text().toLocal8Bit().constData());

    QSqlRecord rec = query.record();

    int tokenIndex                = rec.indexOf("token");
    int qualifiedIndex            = rec.indexOf("qualified");
    int qualifiedForWorkWithIndex = rec.indexOf("qualified_for_work_with");
    int tariffIndex               = rec.indexOf("tariff");

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

        bool ok = query.exec();
        Q_ASSERT_X(ok, "UserDatabase::readUserInfo()", query.lastError().text().toLocal8Bit().constData());
    }

    return res;
}

QList<Account> UserDatabase::readAccounts()
{
    qDebug() << "Reading account";

    QList<Account> res;

    QString str =
        "SELECT id, name "
        "FROM accounts;";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::readAccounts()", query.lastError().text().toLocal8Bit().constData());

    QSqlRecord rec = query.record();

    int idIndex   = rec.indexOf("id");
    int nameIndex = rec.indexOf("name");

    while (query.next())
    {
        Account account;

        account.setId(mSimpleCrypt.decryptToString(query.value(idIndex).toString()));
        account.name = query.value(nameIndex).toString();

        res.append(account);
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

    bool ok = query.exec();
    Q_ASSERT_X(ok, "UserDatabase::writeToken()", query.lastError().text().toLocal8Bit().constData());
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

    bool ok = query.exec();
    Q_ASSERT_X(ok, "UserDatabase::writeToken()", query.lastError().text().toLocal8Bit().constData());
}

void UserDatabase::writeAccounts(const QList<Account>& accounts)
{
    bool ok = db.transaction();
    Q_ASSERT_X(ok, "UserDatabase::writeAccounts()", db.lastError().text().toLocal8Bit().constData());

    QString str =
        "DELETE "
        "FROM accounts;";

    QSqlQuery query1(db);

    ok = query1.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::writeAccounts()", query1.lastError().text().toLocal8Bit().constData());

    QVariantList ids;
    QVariantList names;

    for (int i = 0; i < accounts.size(); ++i)
    {
        const Account& account = accounts.at(i);

        ids << mSimpleCrypt.encryptToString(account.id);
        names << account.name;
    }

    QSqlQuery query2(db);

    query2.prepare(
        "INSERT INTO accounts "
        "(id, name) "
        "VALUES "
        "(:id, :name);"
    );
    query2.bindValue(":id", ids);
    query2.bindValue(":name", names);

    ok = query2.execBatch();
    Q_ASSERT_X(ok, "UserDatabase::writeAccounts()", query2.lastError().text().toLocal8Bit().constData());

    ok = db.commit();
    Q_ASSERT_X(ok, "UserDatabase::writeAccounts()", db.lastError().text().toLocal8Bit().constData());
}
