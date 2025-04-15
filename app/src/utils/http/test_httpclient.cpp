#include "src/utils/http/httpclient.h"

#include <gtest/gtest.h>

#include <QUrlQuery>



TEST(Test_HttpClient, Test_constructor_and_destructor)
{
    const HttpClient client;
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

    const HttpResult result = client.download(url, headers);

    ASSERT_EQ(result.statusCode, 200);
    ASSERT_EQ(
        QString::fromUtf8(result.body)
            .startsWith(
                "{\n  \"args\": {\n    \"hello\": \"test\"\n  }, \n  \"data\": \"\", \n  \"files\": {}, \n  \"form\": "
                "{}, \n  \"headers\": {\n    \"Accept-Encoding\": "
            ),
        true
    );
}

TEST(Test_HttpClient, Test_openInBrowser)
{
    // Nothing
}
