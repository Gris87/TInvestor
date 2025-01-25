#include "test_config.h"



void Test_Config::initTestCase()
{
    // Nothing
}

void Test_Config::cleanupTestCase()
{
    // Nothing
}

void Test_Config::test_case1()
{

}

void Test_Config::test_case2()
{

}

int runTest_Config(int argc, char *argv[])
{
    Test_Config t;

    return QTest::qExec(&t, argc, argv);
}
