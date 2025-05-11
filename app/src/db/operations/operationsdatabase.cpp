#include "src/db/operations/operationsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



OperationsDatabase::OperationsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    IOperationsDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory)
{
    qDebug() << "Create OperationsDatabase";
}

OperationsDatabase::~OperationsDatabase()
{
    qDebug() << "Destroy OperationsDatabase";
}

QList<Operation> OperationsDatabase::readOperations()
{
    qDebug() << "Reading operations from database";

    QList<Operation> res;

    return res;
}

void OperationsDatabase::writeOperations(const QList<Operation>& /*operations*/)
{
    qDebug() << "Writing operations to database";
}

void OperationsDatabase::appendOperations(const QList<Operation>& /*operations*/)
{
    qDebug() << "Appending operations to database";
}
