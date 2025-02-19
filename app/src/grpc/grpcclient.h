#pragma once



#include "src/grpc/igrpcclient.h"

#pragma warning(push)
#pragma warning(disable : 4100 4189 4267)
#include "messages/generated/users.grpc.pb.h"
#pragma warning(pop)

#include "src/storage/user/iuserstorage.h"



class GrpcClient : public IGrpcClient
{
    Q_OBJECT

public:
    explicit GrpcClient(IUserStorage* userStorage, QObject* parent = nullptr);
    ~GrpcClient();

    GrpcClient(const GrpcClient& another)            = delete;
    GrpcClient& operator=(const GrpcClient& another) = delete;

    void connect() override;

private:
    std::shared_ptr<grpc::CallCredentials>                                           mCreds;
    std::unique_ptr<tinkoff::public_::invest::api::contract::v1::UsersService::Stub> mUsersService;

    void getUserInfo();
};
