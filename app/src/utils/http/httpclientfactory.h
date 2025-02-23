#pragma once



#include "src/utils/http/ihttpclientfactory.h"



class HttpClientFactory : public IHttpClientFactory
{
public:
    HttpClientFactory();
    ~HttpClientFactory();

    HttpClientFactory(const HttpClientFactory& another)            = delete;
    HttpClientFactory& operator=(const HttpClientFactory& another) = delete;

    std::shared_ptr<IHttpClient> newInstance() override;
};
