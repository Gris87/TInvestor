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

    mUsersService = tinkoff::UsersService::NewStub(channel);
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

std::shared_ptr<tinkoff::GetInfoResponse> GrpcClient::getUserInfo()
{
    grpc::ClientContext                       context;
    tinkoff::GetInfoRequest                   req;
    std::shared_ptr<tinkoff::GetInfoResponse> resp = std::shared_ptr<tinkoff::GetInfoResponse>(new tinkoff::GetInfoResponse());

    context.set_credentials(mCreds);

    grpc::Status status = mUsersService->GetInfo(&context, req, resp.get());

    if (!status.ok())
    {
        emit authFailed();

        return nullptr;
    }

    return resp;
}

std::shared_ptr<tinkoff::GetAccountsResponse> GrpcClient::getAccounts()
{
    grpc::ClientContext                           context;
    tinkoff::GetAccountsRequest                   req;
    std::shared_ptr<tinkoff::GetAccountsResponse> resp =
        std::shared_ptr<tinkoff::GetAccountsResponse>(new tinkoff::GetAccountsResponse());

    context.set_credentials(mCreds);

    req.set_status(tinkoff::ACCOUNT_STATUS_OPEN);

    grpc::Status status = mUsersService->GetAccounts(&context, req, resp.get());

    if (!status.ok())
    {
        emit authFailed();

        return nullptr;
    }

    return resp;
}
