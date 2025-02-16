#pragma once



#include "src/grpc/igrpcclient.h"



class GrpcClient : public IGrpcClient
{
    Q_OBJECT

public:
    explicit GrpcClient(QObject* parent = nullptr);
    ~GrpcClient();

    GrpcClient(const GrpcClient& another)            = delete;
    GrpcClient& operator=(const GrpcClient& another) = delete;

    void connect() override;
};
