#include "src/grpc/grpcclient.h"

#include <QDebug>
#include <grpcpp/grpcpp.h>

#include "src/grpc/investapiuthenticator.h"



#ifndef USE_SANDBOX
#define ADDRESS "invest-public-api.tinkoff.ru:443"
#else
#define ADDRESS "sandbox-invest-public-api.tinkoff.ru:443"
#endif



GrpcClient::GrpcClient(IUserStorage* userStorage, QObject* parent) :
    IGrpcClient(parent)
{
    qDebug() << "Create GrpcClient";

    mCreds = grpc::MetadataCredentialsFromPlugin(
        std::unique_ptr<grpc::MetadataCredentialsPlugin>(new InvestApiAuthenticator(userStorage))
    );

    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(ADDRESS, grpc::SslCredentials(grpc::SslCredentialsOptions()));

    mUsersService = tinkoff::public_::invest::api::contract::v1::UsersService::NewStub(channel);
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

void GrpcClient::connect()
{
    getUserInfo();

    emit authFailed();
}

void GrpcClient::getUserInfo()
{
    tinkoff::public_::invest::api::contract::v1::GetInfoRequest  req;
    tinkoff::public_::invest::api::contract::v1::GetInfoResponse resp;
    grpc::ClientContext                                      context;
    context.set_credentials(mCreds);

    grpc::Status status = mUsersService->GetInfo(&context, req, &resp);

    qInfo() << status.ok();
    qInfo() << status.error_code();
    qInfo() << status.error_message();
    qInfo() << status.error_details();
    qInfo() << resp.prem_status();
    qInfo() << resp.qual_status();
    qInfo() << resp.qualified_for_work_with().size();
    qInfo() << resp.tariff();
}
