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
    using Headers = QMap<QString, QString>;

    IHttpClient()          = default;
    virtual ~IHttpClient() = default;

    IHttpClient(const IHttpClient& another)            = delete;
    IHttpClient& operator=(const IHttpClient& another) = delete;

    virtual HttpResult download(const QUrl& url, const Headers& headers) = 0;
    virtual bool       openInBrowser(const QUrl& url)                    = 0;
};
