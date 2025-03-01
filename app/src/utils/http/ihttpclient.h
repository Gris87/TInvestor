#pragma once



#include <QByteArray>
#include <QMap>
#include <QUrl>



struct HttpResult
{
    int        statusCode;
    QByteArray body;
};



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

    virtual HttpResult download(const QUrl& url, const Headers& headers) = 0;
};
