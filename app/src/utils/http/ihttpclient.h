#pragma once



#include <QByteArray>
#include <QMap>



class IHttpClient
{
public:
    typedef QMap<QString, QString> Headers;

    IHttpClient()
    {
    }
    virtual ~IHttpClient() = default;

    IHttpClient(const IHttpClient& another)            = delete;
    IHttpClient& operator=(const IHttpClient& another) = delete;

    virtual std::shared_ptr<QByteArray> download(const QString& url, const Headers& headers) = 0;
};
