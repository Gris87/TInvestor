#include <QtTest>



class Test_Config : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_case2();
};

int runTest_Config(int argc, char *argv[]);
