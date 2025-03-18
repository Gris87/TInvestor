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
    ASSERT_EQ(
        QString::fromUtf8(result.body)
            .startsWith("{\n  \"args\": {\n    \"hello\": \"test\"\n  }, \n  \"data\": \"\", \n  \"files\": {}, \n  \"form\": "
                        "{}, \n  \"headers\": {\n    \"Accept-Encoding\": \"gzip, deflate\", \n    \"Accept-Language\": "
                        "\"ru-RU,en,*\", \n    \"Authorization\": \"Bearer debearer\", \n    \"Host\": \"httpbin.org\", \n    "
                        "\"User-Agent\": \"Mozilla/5.0\", \n    \"X-Amzn-Trace-Id\": "),
        true
    );
}

TEST(Test_HttpClient, Test_openInBrowser)
{
    // Nothing
}
