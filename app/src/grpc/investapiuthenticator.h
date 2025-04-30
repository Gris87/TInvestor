#pragma once



#include <grpcpp/grpcpp.h>

#include "src/storage/user/iuserstorage.h"



class InvestApiAuthenticator : public grpc::MetadataCredentialsPlugin
{
public:
    explicit InvestApiAuthenticator(IUserStorage* userStorage) :
        mUserStorage(userStorage)
    {
    }
    ~InvestApiAuthenticator() override = default;

    grpc::Status GetMetadata(
        grpc::string_ref /*serviceUrl*/, // clazy:exclude=function-args-by-ref
        grpc::string_ref /*methodName*/, // clazy:exclude=function-args-by-ref
        const grpc::AuthContext& /*channelAuthContext*/,
        std::multimap<grpc::string, grpc::string>* metadata
    ) override
    {
        const QString bearer = QString("Bearer %1").arg(mUserStorage->getToken());

        metadata->insert(std::make_pair("authorization", bearer.toUtf8().constData()));

        return grpc::Status::OK;
    }

    Q_DISABLE_COPY(InvestApiAuthenticator)

private:
    IUserStorage* mUserStorage;
};
