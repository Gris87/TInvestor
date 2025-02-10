#include "src/utils/objectholder/objectholder.h"

#include <gtest/gtest.h>



struct TestObject
{
    int a;
};



TEST(Test_ObjectHolder, Test_constructor_and_destructor)
{
    TestObject*  obj = new TestObject();
    ObjectHolder objectHolder(obj);
}
