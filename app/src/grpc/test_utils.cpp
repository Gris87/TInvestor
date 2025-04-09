#include "src/grpc/utils.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_GrpcUtils, Test_quotationToFloat)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_NEAR(quotationToFloat(quotation), 1.5f, 0.0001f);

    quotation.set_units(0);
    quotation.set_nano(10000000);

    ASSERT_NEAR(quotationToFloat(quotation), 0.01f, 0.0001f);
}
// NOLINTEND(readability-magic-numbers)
