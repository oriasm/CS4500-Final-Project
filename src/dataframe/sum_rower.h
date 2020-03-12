// language: CwC
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "rower.h"
#include "row.h"

// A rower that prints the sum of all the values in a DataFrame that contains only floats
class SumRower : public Rower
{
public:
  int row_sum;
  int df_sum;

  SumRower()
  {
    row_sum = 0;
    df_sum = 0;
  }

  bool accept(Row &r)
  {
    size_t width = r.width();
    row_sum = 0;
    for (size_t index = 0; index < width; index++)
    {
      int val = r.get_int(index);
      row_sum += val;
    }
    
    df_sum += row_sum;
    return true;
  }

  Rower *clone()
  {
    return new SumRower();
  }

  void join_delete(Rower *other)
  {
  }
};