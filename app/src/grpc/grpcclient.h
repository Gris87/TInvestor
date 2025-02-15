#pragma once



#include "src/grpc/igrpcclient.h"



class GrpcClient : public IGrpcClient
{
public:
    explicit GrpcClient();
    ~GrpcClient();

    GrpcClient(const GrpcClient& another)            = delete;
    GrpcClient& operator=(const GrpcClient& another) = delete;

    void connect() override;
};
