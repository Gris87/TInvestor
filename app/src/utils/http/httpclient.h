#pragma once



#include "src/utils/http/ihttpclient.h"



class HttpClient : public IHttpClient
{
public:
    HttpClient();
    ~HttpClient();

    HttpClient(const HttpClient& another)            = delete;
    HttpClient& operator=(const HttpClient& another) = delete;

    std::shared_ptr<QByteArray> download(const QString& url, const Headers& headers) override;
};
