#pragma once



#include "src/utils/http/ihttpclient.h"



class IHttpClientFactory
{
public:
    IHttpClientFactory()
    {
    }
    virtual ~IHttpClientFactory() = default;

    IHttpClientFactory(const IHttpClientFactory& another)            = delete;
    IHttpClientFactory& operator=(const IHttpClientFactory& another) = delete;

    virtual std::shared_ptr<IHttpClient> newInstance() = 0;
};
