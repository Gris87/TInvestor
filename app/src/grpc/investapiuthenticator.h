#pragma once



#include <grpcpp/grpcpp.h>

#include "src/storage/user/iuserstorage.h"



class InvestApiAuthenticator : public grpc::MetadataCredentialsPlugin
{
public:
    InvestApiAuthenticator(IUserStorage* userStorage) :
        mUserStorage(userStorage)
    {
    }

    grpc::Status GetMetadata(
        grpc::string_ref /*serviceUrl*/, // clazy:exclude=function-args-by-ref
        grpc::string_ref /*methodName*/, // clazy:exclude=function-args-by-ref
        const grpc::AuthContext& /*channelAuthContext*/,
        std::multimap<grpc::string, grpc::string>* metadata
    ) override
    {
        QString bearer = QString("Bearer %1").arg(mUserStorage->getToken());

        metadata->insert(std::make_pair("authorization", bearer.toUtf8().constData()));

        return grpc::Status::OK;
    }

    IUserStorage* mUserStorage;
};
