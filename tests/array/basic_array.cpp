#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../test_utils.cpp"
#include "../../src/utilities/array.h"

void bool_arr()
{
  // length and push tests
  BoolArray *bools = new BoolArray();
  GT_EQUALS(bools->length(), 0);
  BoolArray *bools2 = new BoolArray();
  for (size_t i = 0; i < 5000; i++)
  {
    bools->push(i % 2);
    bools2->push(i % 2);
  }
  GT_EQUALS(bools->length(), 5000);

  // check equality
  GT_TRUE(bools->equals(bools2));
  bools2->push(0);
  GT_FALSE(bools->equals(bools2));

  // value checks
  GT_FALSE(bools->get(0));
  GT_TRUE(bools->get(1));
  GT_FALSE(bools->get(500));
  GT_TRUE(bools->get(501));
  GT_FALSE(bools->get(2500));
  GT_TRUE(bools->get(2501));
  GT_FALSE(bools->get(4998));
  GT_TRUE(bools->get(4999));

  // swap values
  bool b0 = bools->set(0, true);
  bool b1 = bools->set(1, false);
  bool b500 = bools->set(500, true);
  bool b501 = bools->set(501, false);
  bool b2500 = bools->set(2500, true);
  bool b2501 = bools->set(2501, false);
  bool b4998 = bools->set(4998, true);
  bool b4999 = bools->set(4999, false);

  // check that correct value has been extracted
  GT_FALSE(b0);
  GT_FALSE(b500);
  GT_FALSE(b2500);
  GT_FALSE(b4998);
  GT_TRUE(b1);
  GT_TRUE(b501);
  GT_TRUE(b2501);
  GT_TRUE(b4999);

  // check that values have been swapped
  GT_TRUE(bools->get(0));
  GT_FALSE(bools->get(1));
  GT_TRUE(bools->get(500));
  GT_FALSE(bools->get(501));
  GT_TRUE(bools->get(2500));
  GT_FALSE(bools->get(2501));
  GT_TRUE(bools->get(4998));
  GT_FALSE(bools->get(4999));

  // extreme size test
  BoolArray *big_bools = new BoolArray();
  for (size_t i = 0; i < 1000000; i++)
  {
    big_bools->push(true);
  }
  GT_EQUALS(big_bools->length(), 1000000);

  // clear from memory
  delete bools2;
  delete bools;
  delete big_bools;

  exit(0);
}

TEST(W1, bool_arr)
{
  ASSERT_EXIT_ZERO(bool_arr);
}

void float_arr()
{
  // length and push tests
  FloatArray *floats = new FloatArray();
  GT_EQUALS(floats->length(), 0);
  FloatArray *floats2 = new FloatArray();
  for (size_t i = 0; i < 5000; i++)
  {
    floats->push(i * 1.1);
    floats2->push(i * 1.1);
  }
  GT_EQUALS(floats->length(), 5000);

  // check equality
  GT_TRUE(floats->equals(floats2));
  floats2->push(0);
  GT_FALSE(floats->equals(floats2));

  // value checks
  GT_TRUE(same_float(floats->get(0), 0));
  GT_TRUE(same_float(floats->get(1), 1 * 1.1));
  GT_TRUE(same_float(floats->get(500), 500 * 1.1));
  GT_TRUE(same_float(floats->get(501), 501 * 1.1));
  GT_TRUE(same_float(floats->get(2500), 2500 * 1.1));
  GT_TRUE(same_float(floats->get(2501), 2501 * 1.1));
  GT_TRUE(same_float(floats->get(4998), 4998 * 1.1));
  GT_TRUE(same_float(floats->get(4999), 4999 * 1.1));

  // swap values
  float f0 = floats->set(0, 0.123);
  float f1 = floats->set(1, 1.123);
  float f500 = floats->set(500, 500.123);
  float f501 = floats->set(501, 501.123);
  float f2500 = floats->set(2500, 2500.123);
  float f2501 = floats->set(2501, 2501.123);
  float f4998 = floats->set(4998, 4998.123);
  float f4999 = floats->set(4999, 4999.123);

  // check that correct value has been extracted
  GT_TRUE(same_float(f0, 0));
  GT_TRUE(same_float(f1, 1 * 1.1));
  GT_TRUE(same_float(f500, 500 * 1.1));
  GT_TRUE(same_float(f501, 501 * 1.1));
  GT_TRUE(same_float(f2500, 2500 * 1.1));
  GT_TRUE(same_float(f2501, 2501 * 1.1));
  GT_TRUE(same_float(f4998, 4998 * 1.1));
  GT_TRUE(same_float(f4999, 4999 * 1.1));

  // check that values have been swapped
  GT_TRUE(same_float(floats->get(0), 0.123));
  GT_TRUE(same_float(floats->get(1), 1.123));
  GT_TRUE(same_float(floats->get(500), 500.123));
  GT_TRUE(same_float(floats->get(501), 501.123));
  GT_TRUE(same_float(floats->get(2500), 2500.123));
  GT_TRUE(same_float(floats->get(2501), 2501.123));
  GT_TRUE(same_float(floats->get(4998), 4998.123));
  GT_TRUE(same_float(floats->get(4999), 4999.123));

  // extreme size test
  FloatArray *big_floats = new FloatArray();
  for (size_t i = 0; i < 1000000; i++)
  {
    big_floats->push(i * 2.987654321);
  }
  GT_EQUALS(big_floats->length(), 1000000);

  // clear from memory
  delete big_floats;
  delete floats2;
  delete floats;

  exit(0);
}

TEST(W1, float_arr)
{
  ASSERT_EXIT_ZERO(float_arr);
}

void int_arr()
{
  // length and push tests
  IntArray *ints = new IntArray();
  GT_EQUALS(ints->length(), 0);
  IntArray *ints2 = new IntArray();
  for (size_t i = 0; i < 5000; i++)
  {
    ints->push(i * 2);
    ints2->push(i * 2);
  }
  GT_EQUALS(ints->length(), 5000);

  // check equality
  GT_TRUE(ints->equals(ints2));
  ints2->push(0);
  GT_FALSE(ints->equals(ints2));

  // value checks
  GT_EQUALS(ints->get(0), 0);
  GT_EQUALS(ints->get(1), 2);
  GT_EQUALS(ints->get(500), 1000);
  GT_EQUALS(ints->get(501), 1002);
  GT_EQUALS(ints->get(2500), 5000);
  GT_EQUALS(ints->get(2501), 5002);
  GT_EQUALS(ints->get(4998), 9996);
  GT_EQUALS(ints->get(4999), 9998);

  // swap values
  int i0 = ints->set(0, 10);
  int i1 = ints->set(1, 11);
  int i500 = ints->set(500, 510);
  int i501 = ints->set(501, 511);
  int i2500 = ints->set(2500, 2510);
  int i2501 = ints->set(2501, 2511);
  int i4998 = ints->set(4998, 5008);
  int i4999 = ints->set(4999, 5009);

  // check that correct value has been extracted
  GT_EQUALS(i0, 0);
  GT_EQUALS(i1, 2);
  GT_EQUALS(i500, 1000);
  GT_EQUALS(i501, 1002);
  GT_EQUALS(i2500, 5000);
  GT_EQUALS(i2501, 5002);
  GT_EQUALS(i4998, 9996);
  GT_EQUALS(i4999, 9998);

  // check that values have been swapped
  GT_EQUALS(ints->get(0), 10);
  GT_EQUALS(ints->get(1), 11);
  GT_EQUALS(ints->get(500), 510);
  GT_EQUALS(ints->get(501), 511);
  GT_EQUALS(ints->get(2500), 2510);
  GT_EQUALS(ints->get(2501), 2511);
  GT_EQUALS(ints->get(4998), 5008);
  GT_EQUALS(ints->get(4999), 5009);

  // extreme size test
  IntArray *big_ints = new IntArray();
  for (size_t i = 0; i < 1000000; i++)
  {
    big_ints->push(i * 10);
  }
  GT_EQUALS(big_ints->length(), 1000000);

  // clear from memory
  delete big_ints;
  delete ints2;
  delete ints;

  exit(0);
}

TEST(W1, int_arr)
{
  ASSERT_EXIT_ZERO(int_arr);
}

void string_arr()
{
  // length and push tests
  StringArray *strings = new StringArray();
  GT_EQUALS(strings->length(), 0);
  StringArray *strings2 = new StringArray();
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
    strings->push(str);
    strings2->push(str);
  }

  // check equality
  String *extra_string = new String("EXTRA STRING");
  GT_TRUE(strings->equals(strings2));
  strings2->push(extra_string);
  GT_FALSE(strings->equals(strings2));

  // initialize comparison strings
  String *even = new String("even");
  String *odd = new String("odd");

  // value checks
  GT_TRUE(strings->get(0)->equals(even));
  GT_TRUE(strings->get(1)->equals(odd));
  GT_TRUE(strings->get(500)->equals(even));
  GT_TRUE(strings->get(501)->equals(odd));
  GT_TRUE(strings->get(2500)->equals(even));
  GT_TRUE(strings->get(2501)->equals(odd));
  GT_TRUE(strings->get(4998)->equals(even));
  GT_TRUE(strings->get(4999)->equals(odd));

  // initialize test strings
  String *str0 = new String("0");
  String *str1 = new String("1");
  String *str2 = new String("2");
  String *str3 = new String("3");
  String *str4 = new String("4");
  String *str5 = new String("5");
  String *str6 = new String("6");
  String *str7 = new String("7");

  // swap values
  String *s0 = strings->set(0, str0);
  String *s1 = strings->set(1, str1);
  String *s500 = strings->set(500, str2);
  String *s501 = strings->set(501, str3);
  String *s2500 = strings->set(2500, str4);
  String *s2501 = strings->set(2501, str5);
  String *s4998 = strings->set(4998, str6);
  String *s4999 = strings->set(4999, str7);

  // check that correct value has been extracted
  GT_TRUE(s0->equals(even));
  GT_TRUE(s1->equals(odd));
  GT_TRUE(s500->equals(even));
  GT_TRUE(s501->equals(odd));
  GT_TRUE(s2500->equals(even));
  GT_TRUE(s2501->equals(odd));
  GT_TRUE(s4998->equals(even));
  GT_TRUE(s4999->equals(odd));

  // check that values have been swapped
  GT_TRUE(strings->get(0)->equals(str0));
  GT_TRUE(strings->get(1)->equals(str1));
  GT_TRUE(strings->get(500)->equals(str2));
  GT_TRUE(strings->get(501)->equals(str3));
  GT_TRUE(strings->get(2500)->equals(str4));
  GT_TRUE(strings->get(2501)->equals(str5));
  GT_TRUE(strings->get(4998)->equals(str6));
  GT_TRUE(strings->get(4999)->equals(str7));

  // extreme size test
  StringArray *big_strs = new StringArray();
  for (size_t i = 0; i < 1000000; i++)
  {
    String *example_str = new String("example");
    big_strs->push(example_str);
  }
  GT_EQUALS(big_strs->length(), 1000000);

  // clear strings from memory
  for (size_t i = 0; i < 1000000; i++)
  {
    delete big_strs->get(i);
  }
  delete big_strs;

  for (size_t i = 0; i < 5000; i++)
  {
    delete strings->get(i);
  }
  delete extra_string;

  delete strings2;
  delete strings;

  delete even;
  delete odd;

  delete s0;
  delete s1;
  delete s500;
  delete s501;
  delete s2500;
  delete s2501;
  delete s4998;
  delete s4999;

  exit(0);
}

TEST(W1, string_arr)
{
  ASSERT_EXIT_ZERO(string_arr);
}
