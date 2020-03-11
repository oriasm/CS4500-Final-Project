// language: CwC
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "rower.h"
#include "row.h"

// A rower that prints the sum of all the values in a DataFrame that contains only floats
class FilterSumRower : public Rower
{
public:
  int search;

  FilterSumRower(int searching)
  {
    search = searching;
  }

  bool accept(Row &r)
  {
    int row_sum = 0;

    size_t width = r.width();
    for (size_t index = 0; index < width; index++)
    {
      int val = r.get_int(index);
      row_sum += val;
    }

    return row_sum == search;
  }

  Rower *clone()
  {
    return new FilterSumRower(search);
  }

  void join_delete(Rower *other)
  {
  }
};