#include "src/db/user/userdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



#define CRYPT_KEY 0x5EBEB228C0D4D48F



UserDatabase::UserDatabase() :
    IUserDatabase(),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    mSimpleCrypt(CRYPT_KEY)
{
    qDebug() << "Create UserDatabase";

    QString appDir = qApp->applicationDirPath();

    bool ok = QDir().mkpath(appDir + "/data/db/user");
    Q_ASSERT_X(ok, "UserDatabase::UserDatabase()", "Failed to create dir");

    QString dbPath = appDir + "/data/db/user/user.db";
    db.setDatabaseName(dbPath);

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
        "    id         INTEGER PRIMARY KEY, "
        "    token      VARCHAR(255) NOT NULL, "
        "    qualified  BOOLEAN NOT NULL CHECK (qualified IN (0, 1)), "
        "    commission REAL NOT NULL"
        ");";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::createUserTable()", db.lastError().text().toLocal8Bit().constData());
}

User UserDatabase::readUserInfo()
{
    qDebug() << "Reading user info";

    User res;

    QString str = "SELECT token, qualified, commission FROM user WHERE id = 1;";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "UserDatabase::readUserInfo()", db.lastError().text().toLocal8Bit().constData());

    QSqlRecord rec = query.record();

    int tokenIndex      = rec.indexOf("token");
    int qualifiedIndex  = rec.indexOf("qualified");
    int commissionIndex = rec.indexOf("commission");

    if (query.first())
    {
        res.token      = mSimpleCrypt.decryptToString(query.value(tokenIndex).toString());
        res.qualified  = query.value(qualifiedIndex).toLongLong() != 0;
        res.commission = query.value(commissionIndex).toFloat();
    }
    else
    {
        res.token      = "";
        res.qualified  = false;
        res.commission = 0.3f;

        QSqlQuery query(db);
        query.prepare("INSERT INTO user (id, token, qualified, commission) VALUES (?, ?, ?, ?);");
        query.bindValue(0, 1);
        query.bindValue(1, mSimpleCrypt.encryptToString(res.token));
        query.bindValue(2, res.qualified ? 1 : 0);
        query.bindValue(3, res.commission);

        bool ok = query.exec();
        Q_ASSERT_X(ok, "UserDatabase::readUserInfo()", db.lastError().text().toLocal8Bit().constData());
    }

    return res;
}

void UserDatabase::UserDatabase::writeToken(const QString& token)
{
    QSqlQuery query(db);
    query.prepare("UPDATE user SET token = ? WHERE id = 1;");
    query.bindValue(0, mSimpleCrypt.encryptToString(token));

    bool ok = query.exec();
    Q_ASSERT_X(ok, "UserDatabase::writeToken()", db.lastError().text().toLocal8Bit().constData());
}

void UserDatabase::writeUserInfo(const User& user)
{
    QSqlQuery query(db);
    query.prepare("UPDATE user SET qualified = ?, commission = ? WHERE id = 1;");
    query.bindValue(0, user.qualified ? 1 : 0);
    query.bindValue(1, user.commission);

    bool ok = query.exec();
    Q_ASSERT_X(ok, "UserDatabase::writeToken()", db.lastError().text().toLocal8Bit().constData());
}
