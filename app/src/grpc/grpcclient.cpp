#include "src/grpc/grpcclient.h"

#include <QDebug>



GrpcClient::GrpcClient(QObject* parent) :
    IGrpcClient(parent)
{
    qDebug() << "Create GrpcClient";
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

void GrpcClient::connect()
{
    emit authFailed();
}
