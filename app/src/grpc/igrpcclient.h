#pragma once



class IGrpcClient
{
public:
    IGrpcClient()
    {
    }
    virtual ~IGrpcClient() = default;

    IGrpcClient(const IGrpcClient& another)            = delete;
    IGrpcClient& operator=(const IGrpcClient& another) = delete;

    virtual void connect() = 0;
};
