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

    MOCK_METHOD(HttpResult, download, (const QUrl& url, const Headers& headers), (override));
};
