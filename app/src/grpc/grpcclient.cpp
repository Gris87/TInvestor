#include "src/grpc/grpcclient.h"

#include <QDebug>

#include <grpcpp/grpcpp.h>



#ifndef USE_SANDBOX
#define ADDRESS "invest-public-api.tinkoff.ru:443"
#else
#define ADDRESS "sandbox-invest-public-api.tinkoff.ru:443"
#endif



GrpcClient::GrpcClient(QObject* parent) :
    IGrpcClient(parent),
    mUsersService()
{
    qDebug() << "Create GrpcClient";
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

void GrpcClient::connect()
{
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(ADDRESS, grpc::InsecureChannelCredentials());

    mUsersService = tinkoff::pub::invest::api::contract::v1::UsersService::NewStub(channel);

    emit authFailed();
}
