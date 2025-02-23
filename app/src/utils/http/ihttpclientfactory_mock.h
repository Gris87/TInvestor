#pragma once



#include "src/utils/http/ihttpclientfactory.h"

#include <gmock/gmock.h>



class HttpClientFactoryMock : public IHttpClientFactory
{
public:
    HttpClientFactoryMock() :
        IHttpClientFactory()
    {
    }
    ~HttpClientFactoryMock() override = default;

    HttpClientFactoryMock(const HttpClientFactoryMock& another)            = delete;
    HttpClientFactoryMock& operator=(const HttpClientFactoryMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<IHttpClient>, newInstance, (), (override));
};
