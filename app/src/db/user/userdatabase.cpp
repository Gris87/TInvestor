#include "src/db/account/accountdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



const char* password = "M4&T7>nA60Bu}^JZ>9k7qTuQ1}Vj£t+f";



AccountDatabase::AccountDatabase() :
    IAccountDatabase(),
    db(QSqlDatabase::addDatabase("QSQLITE"))
{
    qDebug() << "Create AccountDatabase";

    QString appDir = qApp->applicationDirPath();

    bool ok = QDir().mkpath(appDir + "/data/db/account");
    Q_ASSERT_X(ok, "AccountDatabase::AccountDatabase()", "Failed to create dir");

    QString dbPath = appDir + "/data/db/account/account.db";
    db.setDatabaseName(dbPath);
    db.setPassword(password);

    ok = db.open();
    Q_ASSERT_X(ok, "AccountDatabase::AccountDatabase()", db.lastError().text().toLocal8Bit().constData());

    qInfo() << "Account database" << dbPath << "created";

    createAccountTable();
}

AccountDatabase::~AccountDatabase()
{
    qDebug() << "Destroy AccountDatabase";

    db.close();
}

void AccountDatabase::createAccountTable()
{
    qDebug() << "Create table with account info";

    QString str =
        "CREATE TABLE IF NOT EXISTS account ("
        "    id       INTEGER PRIMARY KEY, "
        "    token    VARCHAR(255)"
        ");";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "AccountDatabase::createAccountTable()", db.lastError().text().toLocal8Bit().constData());
}

Account AccountDatabase::readAccountInfo()
{
    qDebug() << "Reading account info";

    Account res;

    QString str = "SELECT token FROM account WHERE id = 1;";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "AccountDatabase::readAccountInfo()", db.lastError().text().toLocal8Bit().constData());

    QSqlRecord rec = query.record();

    int tokenIndex = rec.indexOf("token");

    if (query.first())
    {
        res.token = query.value(tokenIndex).toString();
    }
    else
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO account (id, token) VALUES (?, ?);");
        query.bindValue(0, 1);
        query.bindValue(1, res.token);

        bool ok = !query.exec();
        Q_ASSERT_X(ok, "AccountDatabase::readAccountInfo()", db.lastError().text().toLocal8Bit().constData());
    }

    return res;
}

void AccountDatabase::AccountDatabase::writeToken(const QString token)
{
    QSqlQuery query(db);
    query.prepare("UPDATE account SET token = ? WHERE id = 1;");
    query.bindValue(0, token);

    bool ok = !query.exec();
    Q_ASSERT_X(ok, "AccountDatabase::writeToken()", db.lastError().text().toLocal8Bit().constData());
}
