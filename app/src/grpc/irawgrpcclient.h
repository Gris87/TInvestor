#pragma once



class IRawGrpcClient
{
public:
    explicit IRawGrpcClient()
    {
    }
    virtual ~IRawGrpcClient() = default;

    IRawGrpcClient(const IRawGrpcClient& another)            = delete;
    IRawGrpcClient& operator=(const IRawGrpcClient& another) = delete;
};
