#include "src/utils/http/httpclient.h"

#include <QDebug>
#include <QDesktopServices>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>



constexpr int HTTP_STATUS_CODE_OK = 200;



HttpClient::HttpClient() :
    IHttpClient()
{
    qDebug() << "Create HttpClient";
}

HttpClient::~HttpClient()
{
    qDebug() << "Destroy HttpClient";
}

HttpResult HttpClient::get(const QUrl& url, const Headers& headers)
{
    qDebug() << "Perform HTTP GET:" << url;

    QNetworkAccessManager manager;
    QNetworkRequest       request(url);

    for (auto it = headers.constBegin(), end = headers.constEnd(); it != end; ++it)
    {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply* reply = manager.get(request);

    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    HttpResult res;

    res.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() == QNetworkReply::NoError && res.statusCode == HTTP_STATUS_CODE_OK)
    {
        res.body = reply->readAll();
    }

    delete reply;

    return res;
}
