#include <QtTest>



class tests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_case2();
};

void tests::initTestCase()
{
    // Nothing
}

void tests::cleanupTestCase()
{
    // Nothing
}

void tests::test_case1()
{

}

void tests::test_case2()
{

}



QTEST_MAIN(tests)

#include "test_config.moc"
