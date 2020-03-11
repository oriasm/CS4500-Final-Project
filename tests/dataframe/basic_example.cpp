#include <gtest/gtest.h>
#include "../../src/dataframe/dataframe.h" 
#include "../test_utils.cpp"

void test() {
  Schema s("II");

  DataFrame df(s);
  Row  r(df.get_schema());
  for(size_t i = 0; i <  1000 * 1000; i++) {
    r.set(0,(int)i);
    r.set(1,(int)i+1);
    df.add_row(r);
  }
  GT_EQUALS(df.get_int((size_t)0,1), 1);
  exit(0);
}
