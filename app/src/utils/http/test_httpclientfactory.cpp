#include "src/utils/http/httpclientfactory.h"

#include <gtest/gtest.h>



TEST(Test_HttpClientFactory, Test_constructor_and_destructor)
{
    HttpClientFactory factory;
}

TEST(Test_HttpClientFactory, Test_newInstance)
{
    HttpClientFactory factory;

    std::shared_ptr<IHttpClient> file = factory.newInstance();
    ASSERT_TRUE(file != nullptr);
}
