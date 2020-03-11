#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../test_utils.cpp"
#include "../../src/dataframe/column.h"

void bool_column()
{
    // length and push tests
    BoolColumn *bools = new BoolColumn();
    GT_EQUALS(bools->size(), 0);
    for (size_t i = 0; i < 5000; i++)
    {
        bools->push_back(i % 2);
    }
    GT_EQUALS(bools->size(), 5000);

    // value checks
    GT_FALSE(bools->get(0));
    GT_TRUE(bools->get(1));
    GT_FALSE(bools->get(4998));
    GT_TRUE(bools->get(4999));

    // swap values
    bools->set(0, true);
    bools->set(1, false);
    bools->set(4998, true);
    bools->set(4999, false);

    // check swapped values
    GT_TRUE(bools->get(0));
    GT_FALSE(bools->get(1));
    GT_TRUE(bools->get(4998));
    GT_FALSE(bools->get(4999));

    // variable arg constructor
    BoolColumn *more_bools = new BoolColumn(3, true, false, true);

    // value checks
    GT_TRUE(more_bools->get(0));
    GT_TRUE(!more_bools->get(1));
    GT_TRUE(more_bools->get(2));

    delete bools;
    delete more_bools;

    exit(0);
}

TEST(W1, bool_column)
{
    ASSERT_EXIT_ZERO(bool_column);
}

void float_column()
{
    // length and push tests
    FloatColumn *floats = new FloatColumn();
    GT_EQUALS(floats->size(), 0);
    for (size_t i = 0; i < 5000; i++)
    {
        floats->push_back(i * 5.5);
    }
    GT_EQUALS(floats->size(), 5000);

    // value checks
    GT_TRUE(same_float(floats->get(0), 0 * 5.5));
    GT_TRUE(same_float(floats->get(1), 1 * 5.5));
    GT_TRUE(same_float(floats->get(4998), 4998 * 5.5));
    GT_TRUE(same_float(floats->get(4999), 4999 * 5.5));

    // swap values
    floats->set(0, 0.123);
    floats->set(1, 1.123);
    floats->set(4998, 4998.123);
    floats->set(4999, 4999.123);

    // check swapped values
    GT_TRUE(same_float(floats->get(0), 0.123));
    GT_TRUE(same_float(floats->get(1), 1.123));
    GT_TRUE(same_float(floats->get(4998), 4998.123));
    GT_TRUE(same_float(floats->get(4999), 4999.123));

    // variable arg constructor
    FloatColumn *more_floats = new FloatColumn(3, 0.123, 1.234, 2.345);

    // value checks 
    GT_TRUE(same_float(more_floats->get(0), 0.123));
    GT_TRUE(same_float(more_floats->get(1), 1.234));
    GT_TRUE(same_float(more_floats->get(2), 2.345));

    delete floats;
    delete more_floats;

    exit(0);
}

TEST(W1, float_column)
{
    ASSERT_EXIT_ZERO(float_column);
}

void int_column()
{
    // length and push tests
    IntColumn *ints = new IntColumn();
    GT_EQUALS(ints->size(), 0);
    for (size_t i = 0; i < 5000; i++)
    {
        ints->push_back(i);
    }
    GT_EQUALS(ints->size(), 5000);

    // value checks
    GT_EQUALS(ints->get(0), 0);
    GT_EQUALS(ints->get(1), 1);
    GT_EQUALS(ints->get(4998), 4998);
    GT_EQUALS(ints->get(4999), 4999);

    // swap values
    ints->set(0, 10);
    ints->set(1, 20);
    ints->set(4998, 30);
    ints->set(4999, 40);

    // check swapped values
    GT_EQUALS(ints->get(0), 10);
    GT_EQUALS(ints->get(1), 20);
    GT_EQUALS(ints->get(4998), 30);
    GT_EQUALS(ints->get(4999), 40);

    // variable arg constructor
    IntColumn *more_ints = new IntColumn(3, 100, 200, 300);

    // value checks
    GT_EQUALS(more_ints->get(0), 100);
    GT_EQUALS(more_ints->get(1), 200);
    GT_EQUALS(more_ints->get(2), 300);

    delete ints;
    delete more_ints;

    exit(0);
}

TEST(W1, int_column)
{
    ASSERT_EXIT_ZERO(int_column);
}

void string_column()
{
    // length and push tests
    StringColumn *strings = new StringColumn();
    GT_EQUALS(strings->size(), 0);
    for (size_t i = 0; i < 5000; i++)
    {
        String *str;
        if (i % 2 == 0)
        {
            str = new String("even");
        }
        else
        {
            str = new String("odd");
        }
        strings->push_back(str);
        delete str;
    }
    GT_EQUALS(strings->size(), 5000);

    String *even = new String("even");
    String *odd = new String("odd");

    // value checks
    GT_TRUE(strings->get(0)->equals(even));
    GT_TRUE(strings->get(1)->equals(odd));
    GT_TRUE(strings->get(4998)->equals(even));
    GT_TRUE(strings->get(4999)->equals(odd));

    // swap values
    String *swap = new String("swap");
    strings->set(0, swap);
    strings->set(1, swap);
    strings->set(4998, swap);
    strings->set(4999, swap);

    // // check swapped values
    GT_TRUE(strings->get(0)->equals(swap));
    GT_TRUE(strings->get(1)->equals(swap));
    GT_TRUE(strings->get(4998)->equals(swap));
    GT_TRUE(strings->get(4999)->equals(swap));

    String *str1 = new String("one");
    String *str2 = new String("two");
    String *str3 = new String("three");

    // variable arg constructor
    StringColumn *more_string = new StringColumn(3, str1, str2, str3);

    // value checks
    GT_TRUE(more_string->get(0)->equals(str1));
    GT_TRUE(more_string->get(1)->equals(str2));
    GT_TRUE(more_string->get(2)->equals(str3));

    delete str1;
    delete str2;
    delete str3;

    delete swap;

    delete even;
    delete odd;

    delete more_string;
    delete strings;

    exit(0);
}

TEST(W1, string_column)
{
    ASSERT_EXIT_ZERO(string_column);
}

void basic_column()
{
    IntColumn *ints = new IntColumn();
    BoolColumn *bools = new BoolColumn();
    FloatColumn *floats = new FloatColumn();
    StringColumn *strings = new StringColumn();

    // test type converters
    GT_TRUE(ints->as_int()->equals(ints));
    GT_TRUE(bools->as_bool()->equals(bools));
    GT_TRUE(floats->as_float()->equals(floats));
    GT_TRUE(strings->as_string()->equals(strings));

    // test type getters
    GT_EQUALS(ints->get_type(), 'I');
    GT_EQUALS(bools->get_type(), 'B');
    GT_EQUALS(floats->get_type(), 'F');
    GT_EQUALS(strings->get_type(), 'S');

    delete ints;
    delete bools;
    delete floats;
    delete strings;

    exit(0);
}

TEST(W1, basic_column)
{
    ASSERT_EXIT_ZERO(basic_column);
}