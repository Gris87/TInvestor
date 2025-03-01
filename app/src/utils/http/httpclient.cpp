#include "src/utils/http/httpclient.h"

#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>



HttpClient::HttpClient() :
    IHttpClient()
{
    qDebug() << "Create HttpClient";
}

HttpClient::~HttpClient()
{
    qDebug() << "Destroy HttpClient";
}

HttpResult HttpClient::download(const QUrl& url, const Headers& headers)
{
    QNetworkAccessManager manager;
    QNetworkRequest       request(url);

    for (auto i = headers.cbegin(), end = headers.cend(); i != end; ++i)
    {
        request.setRawHeader(i.key().toUtf8(), i.value().toUtf8());
    }

    QNetworkReply* reply = manager.get(request);

    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    HttpResult res;

    res.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() == QNetworkReply::NoError && res.statusCode == 200)
    {
        res.body = reply->readAll();
    }

    return res;
}
