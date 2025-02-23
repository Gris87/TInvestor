#pragma once



#include <QByteArray>



class IHttpClient
{
public:
    IHttpClient()
    {
    }
    virtual ~IHttpClient() = default;

    IHttpClient(const IHttpClient& another)            = delete;
    IHttpClient& operator=(const IHttpClient& another) = delete;

    virtual std::shared_ptr<QByteArray> download(const QString& url) = 0;
};
