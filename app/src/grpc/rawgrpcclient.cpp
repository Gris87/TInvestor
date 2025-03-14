#include "src/grpc/rawgrpcclient.h"

#include <QDebug>



RawGrpcClient::RawGrpcClient() :
    IRawGrpcClient()
{
    qDebug() << "Create RawGrpcClient";
}

RawGrpcClient::~RawGrpcClient()
{
    qDebug() << "Destroy RawGrpcClient";
}
