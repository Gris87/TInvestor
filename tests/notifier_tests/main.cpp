#include <QApplication>

#include <gmock/gmock.h>
#include <gtest/gtest.h>



int main(int argc, char* argv[])
{
    const QApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
