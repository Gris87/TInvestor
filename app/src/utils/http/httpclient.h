#pragma once



#include "src/utils/http/ihttpclient.h"



class HttpClient : public IHttpClient
{
public:
    HttpClient();
    ~HttpClient() override;

    HttpClient(const HttpClient& another)            = delete;
    HttpClient& operator=(const HttpClient& another) = delete;

    HttpResult download(const QUrl& url, const Headers& headers) override;
    bool       openInBrowser(const QUrl& url) override;
};
