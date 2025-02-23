#pragma once



#include "src/utils/http/ihttpclient.h"

#include <gmock/gmock.h>



class HttpClientMock : public IHttpClient
{
public:
    HttpClientMock() :
        IHttpClient()
    {
    }
    ~HttpClientMock() override = default;

    HttpClientMock(const HttpClientMock& another)            = delete;
    HttpClientMock& operator=(const HttpClientMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<QByteArray>, download, (const QString& url), (override));
};
