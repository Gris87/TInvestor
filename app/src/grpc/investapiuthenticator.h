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
        grpc::string_ref /*serviceUrl*/,
        grpc::string_ref /*methodName*/,
        const grpc::AuthContext& /*channelAuthContext*/,
        std::multimap<grpc::string, grpc::string>* metadata
    ) override
    {
        metadata->insert(
            std::make_pair(
                "authorization", "Bearer t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ"
            )
        );

        return grpc::Status::OK;
    }

    IUserStorage* mUserStorage;
};
