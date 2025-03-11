#include "src/utils/http/httpclient.h"

#include <gtest/gtest.h>

#include <QUrlQuery>



TEST(Test_HttpClient, Test_constructor_and_destructor)
{
    HttpClient client;
}

TEST(Test_HttpClient, Test_download)
{
    HttpClient client;

    QUrl url = QUrl("https://httpbin.org/anything");

    QUrlQuery query;
    query.addQueryItem("hello", "test");
    url.setQuery(query.query());

    IHttpClient::Headers headers;
    headers["Authorization"] = "Bearer debearer";

    HttpResult result = client.download(url, headers);

    ASSERT_EQ(result.statusCode, 200);
}
