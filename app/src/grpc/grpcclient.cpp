#include "src/grpc/grpcclient.h"

#include <QDebug>



#ifndef USE_SANDBOX
#define ADDRESS "invest-public-api.tinkoff.ru"
#else
#define ADDRESS "sandbox-invest-public-api.tinkoff.ru"
#endif



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
