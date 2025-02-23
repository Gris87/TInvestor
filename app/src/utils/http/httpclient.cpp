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

std::shared_ptr<QByteArray> HttpClient::download(const QString& url)
{
    QNetworkAccessManager manager;
    QNetworkRequest       request(url);
    QNetworkReply*        reply = manager.get(request);

    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    qInfo() << reply->error() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != QNetworkReply::NoError || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
    {
        return nullptr;
    }

    return std::shared_ptr<QByteArray>(new QByteArray(reply->readAll()));
}
