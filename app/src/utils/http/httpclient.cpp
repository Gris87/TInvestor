#include "src/utils/http/httpclient.h"

#include <QDebug>



HttpClient::HttpClient() :
    IHttpClient()
{
    qDebug() << "Create HttpClient";
}

HttpClient::~HttpClient()
{
    qDebug() << "Destroy HttpClient";
}

std::shared_ptr<QByteArray> HttpClient::download(const QString& /*url*/)
{
    return nullptr;
}
