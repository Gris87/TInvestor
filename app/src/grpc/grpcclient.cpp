#include "src/grpc/grpcclient.h"

#include <QDebug>



GrpcClient::GrpcClient() :
    IGrpcClient()
{
    qDebug() << "Create GrpcClient";
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

void GrpcClient::connect()
{
}
