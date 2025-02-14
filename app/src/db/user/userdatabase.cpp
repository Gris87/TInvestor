#include "src/db/user/userdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



const char* password = "M4&T7>nA60Bu}^JZ>9k7qTuQ1}Vj£t+f";



UserDatabase::UserDatabase() :
    IUserDatabase(),
    db(QSqlDatabase::addDatabase("QSQLITE"))
{
    qDebug() << "Create UserDatabase";

    QString appDir = qApp->applicationDirPath();

    bool ok = QDir().mkpath(appDir + "/data/db/user");
    Q_ASSERT_X(ok, "UserDatabase::UserDatabase()", "Failed to create dir");

    QString dbPath = appDir + "/data/db/user/user.db";
    db.setDatabaseName(dbPath);
    db.setPassword(password);

    ok = db.open();
    Q_ASSERT_X(ok, "UserDatabase::UserDatabase()", db.lastError().text().toLocal8Bit().constData());

    qInfo() << "User database" << dbPath << "created";

    createUserTable();
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
        "    id       INTEGER PRIMARY KEY, "
        "    token    VARCHAR(255)"
        ");";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::createUserTable()", db.lastError().text().toLocal8Bit().constData());
}

User UserDatabase::readUserInfo()
{
    qDebug() << "Reading user info";

    User res;

    QString str = "SELECT token FROM user WHERE id = 1;";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::readUserInfo()", db.lastError().text().toLocal8Bit().constData());

    QSqlRecord rec = query.record();

    int tokenIndex = rec.indexOf("token");

    if (query.first())
    {
        res.token = query.value(tokenIndex).toString();
    }
    else
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO user (id, token) VALUES (?, ?);");
        query.bindValue(0, 1);
        query.bindValue(1, res.token);

        bool ok = !query.exec();
        Q_ASSERT_X(ok, "UserDatabase::readUserInfo()", db.lastError().text().toLocal8Bit().constData());
    }

    return res;
}

void UserDatabase::UserDatabase::writeToken(const QString token)
{
    QSqlQuery query(db);
    query.prepare("UPDATE user SET token = ? WHERE id = 1;");
    query.bindValue(0, token);

    bool ok = !query.exec();
    Q_ASSERT_X(ok, "UserDatabase::writeToken()", db.lastError().text().toLocal8Bit().constData());
}
