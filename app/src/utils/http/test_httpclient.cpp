#include "src/utils/http/httpclient.h"

#include <gtest/gtest.h>

#include <QThread>
#include <QUrlQuery>



// NOLINTBEGIN(readability-magic-numbers)
class Test_HttpClient : public ::testing::Test
{
protected:
    void SetUp() override
    {
        client = new HttpClient();
    }

    void TearDown() override
    {
        delete client;
    }

    HttpClient* client;
};



TEST_F(Test_HttpClient, Test_constructor_and_destructor)
{
}

TEST_F(Test_HttpClient, Test_get)
{
    QUrl url = QUrl("https://httpbin.org/anything");

    QUrlQuery query;
    query.addQueryItem("hello", "test");
    url.setQuery(query.query());

    IHttpClient::Headers headers;
    headers["Authorization"] = "Bearer debearer";

    for (int attempts = 0; attempts < 10; ++attempts)
    {
        const HttpResult result = client->get(url, headers);

        if (result.statusCode == 200)
        {
            ASSERT_EQ(
                QString::fromUtf8(result.body)
                    .startsWith(
                        "{\n  \"args\": {\n    \"hello\": \"test\"\n  }, \n  \"data\": \"\", \n  \"files\": {}, \n  \"form\": "
                        "{}, \n  \"headers\": {\n    \"Accept-Encoding\": "
                    ),
                true
            );

            break;
        }

        QThread::msleep(5000);
    }
}

TEST_F(Test_HttpClient, Test_openInBrowser)
{
    // Nothing
}
// NOLINTEND(readability-magic-numbers)
