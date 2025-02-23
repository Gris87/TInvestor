#include "src/utils/http/httpclientfactory.h"

#include <QDebug>

#include "src/utils/http/httpclient.h"



HttpClientFactory::HttpClientFactory() :
    IHttpClientFactory()
{
    qDebug() << "Create HttpClientFactory";
}

HttpClientFactory::~HttpClientFactory()
{
    qDebug() << "Destroy HttpClientFactory";
}

std::shared_ptr<IHttpClient> HttpClientFactory::newInstance()
{
    return std::shared_ptr<IHttpClient>(new HttpClient());
}
