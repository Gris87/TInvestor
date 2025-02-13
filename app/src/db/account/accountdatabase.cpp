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
