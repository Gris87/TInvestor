#pragma once



#include <QObject>

#pragma warning(push)
#pragma warning(disable : 4100 4189 4267)
#include "messages/generated/users.grpc.pb.h"
#pragma warning(pop)



using namespace tinkoff::public_::invest::api::contract::v1;



class IGrpcClient : public QObject
{
    Q_OBJECT

public:
    explicit IGrpcClient(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    virtual ~IGrpcClient() = default;

    IGrpcClient(const IGrpcClient& another)            = delete;
    IGrpcClient& operator=(const IGrpcClient& another) = delete;

    virtual std::shared_ptr<GetInfoResponse> getUserInfo() = 0;

signals:
    void authFailed();
};
