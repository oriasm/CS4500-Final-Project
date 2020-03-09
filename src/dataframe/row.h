//lang::CwC
#pragma once
#include "fielder.h"
#include "schema.h"
#include "../utilities/array.h"
#include <iostream>

/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object
{
public:
  // An Array of Array objects, each of a specific type holding one value
  ObjectArray *valueArrays_;
  // Number of columns(values) in this row
  size_t row_width_;
  // Index in DataFrame
  size_t row_idx;
  // String buffer representing the types in this row
  StrBuff *schema_;

  /** Build a row following a schema. */
  Row(Schema &scm)
  {
    valueArrays_ = new ObjectArray();
    row_width_ = scm.width();
    schema_ = new StrBuff();
    row_idx = 0;

    for (size_t index = 0; index < row_width_; index++)
    {
      // Turn char into null terminated char * 
      char col = scm.col_type(index);
      char *ptr = new char[2];
      ptr[0] = col;
      ptr[1] = '\0';
      // Add to schema buffer
      schema_ = &schema_->c(ptr);
      delete[] ptr;
      // For each column type, instantiate empty array of that type
      switch (col)
      {
      case 'I':
        valueArrays_->push(new IntArray());
        break;
      case 'F':
        valueArrays_->push(new FloatArray());
        break;
      case 'B':
        valueArrays_->push(new BoolArray());
        break;
      case 'S':
        valueArrays_->push(new StringArray());
        break;
      default:
        printf("Invalid schema at column %zu\n", index);
        exit(1);
      }
    }
  }

  /* Clears a Row and its contents from memory */
  ~Row()
  {
    String *types = schema_->get();
    delete types;
    delete schema_;
    // free each array inside valueArrays
    for (size_t index = 0; index < row_width_; index++)
    {
      Object *valueArr = valueArrays_->get(index);
      Array *arr = dynamic_cast<Array *>(valueArr);
      StringArray *toDelete = dynamic_cast<StringArray *>(arr);
      if (toDelete) {
        if (toDelete->length() > 0) {
          delete toDelete->get(0);
        }
      }
      delete arr;

    }
    // Free valueArrays
    delete valueArrays_;
  }

  /** Setters: set the given column with the given value. */
  void set(size_t col, int val)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is an integer
    assert(type == 'I');

    IntArray *toUpdate = dynamic_cast<IntArray *>(valueArrays_->get(col));
    if (toUpdate)
    {
      if (toUpdate->length() == 0)
      {
        toUpdate->push(val);
      }
      else
      {
        toUpdate->set(0, val);
      }
      // Size should be 1
      assert(toUpdate->length() == 1);
    }
  }

  /* Sets a value in a row */
  void set(size_t col, float val)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is a float
    assert(type == 'F');

    FloatArray *toUpdate = dynamic_cast<FloatArray *>(valueArrays_->get(col));
    if (toUpdate)
    {
      if (toUpdate->length() == 0)
      {
        toUpdate->push(val);
      }
      else
      {
        toUpdate->set(0, val);
      }
      // Size should be 1
      assert(toUpdate->length() == 1);
    }
  }

  /* Sets a value in a row */
  void set(size_t col, bool val)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is a boolean
    assert(type == 'B');

    BoolArray *toUpdate = dynamic_cast<BoolArray *>(valueArrays_->get(col));
    if (toUpdate)
    {
      if (toUpdate->length() == 0)
      {
        toUpdate->push(val);
      }
      else
      {
        toUpdate->set(0, val);
      }
      // Size should be 1
      assert(toUpdate->length() == 1);
    }
  }

  /** Acquire ownership of the string. */
  void set(size_t col, String *val)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is a string
    assert(type == 'S');

    StringArray *toUpdate = dynamic_cast<StringArray *>(valueArrays_->get(col));
    if (toUpdate)
    {
      // Acquire ownership of string
      String *copy = new String(*val);

      if (toUpdate->length() == 0)
      {
        toUpdate->push(copy);
      }
      else
      {
        String* toDelete = toUpdate->set(0, copy);
        delete toDelete;
      }
      // Size should be 1
      assert(toUpdate->length() == 1);
    }
  }

  /** Set/get the index of this row (ie. its position in the dataframe. This is
   *  only used for informational purposes, unused otherwise */
  void set_idx(size_t idx)
  {
    row_idx = idx;
  }

  /* Gets the row index */
  size_t get_idx()
  {
    return row_idx;
  }

  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is an integer
    assert(type = 'I');

    IntArray *toGet = dynamic_cast<IntArray *>(valueArrays_->get(col));
    assert(toGet);
    return toGet->get(0);
  }

  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  bool get_bool(size_t col)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is a boolean
    assert(type = 'B');

    BoolArray *toGet = dynamic_cast<BoolArray *>(valueArrays_->get(col));
    assert(toGet);
    return toGet->get(0);
  }

  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  float get_float(size_t col)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is a float
    assert(type = 'F');

    FloatArray *toGet = dynamic_cast<FloatArray *>(valueArrays_->get(col));
    assert(toGet);
    return toGet->get(0);
  }

  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  String *get_string(size_t col)
  {
    // column index exists
    assert(col < row_width_);
    char type = schema_->val_[col];
    // column is a String
    assert(type = 'S');

    StringArray *toGet = dynamic_cast<StringArray *>(valueArrays_->get(col));
    assert(toGet);
    return toGet->get(0);
  }

  /** Number of fields in the row. */
  size_t width()
  {
    return row_width_;
  }

  /** Type of the field at the given position. An idx >= width is
    *  undefined. */
  char col_type(size_t idx)
  {
    // column index should exist
    assert(idx < row_width_);
    return schema_->val_[idx];
  }

  /** Given a Fielder, visit every field of this row. The first argument is
    * index of the row in the dataframe.
    * Calling this method before the row's fields have been set is undefined. */
  void visit(size_t idx, Fielder &f)
  {
    f.start(idx);
    for (size_t index = 0; index < row_width_; index++)
    {
      Object *uncast_arr = valueArrays_->get(index);
      char type = schema_->val_[index];

      switch (type)
      {
      case 'I':
        visit_int(f, uncast_arr);
      case 'F':
        visit_float(f, uncast_arr);
      case 'B':
        visit_bool(f, uncast_arr);
      case 'S':
        visit_string(f, uncast_arr);
      }
    }
    f.done();
  }

  /* Visits an int */
  void visit_int(Fielder &f, Object *valueArray)
  {
    IntArray *values = dynamic_cast<IntArray *>(valueArray);
    if (values)
    {
      // Only rows filled with values should have visitors
      assert(values->length() != 0);
      f.accept(values->get(0));
    }
  }

  /* Visits a float */
  void visit_float(Fielder &f, Object *valueArray)
  {
    FloatArray *values = dynamic_cast<FloatArray *>(valueArray);
    if (values)
    {
      // Only rows filled with values should have visitors
      assert(values->length() != 0);
      f.accept(values->get(0));
    }
  }

  /* Visits a bool */
  void visit_bool(Fielder &f, Object *valueArray)
  {
    BoolArray *values = dynamic_cast<BoolArray *>(valueArray);
    if (values)
    {
      // Only rows filled with values should have visitors
      assert(values->length() != 0);
      f.accept(values->get(0));
    }
  }

  /* Visits a String */
  void visit_string(Fielder &f, Object *valueArray)
  {
    StringArray *values = dynamic_cast<StringArray *>(valueArray);
    if (values)
    {
      // Only rows filled with values should have visitors
      assert(values->length() != 0);
      f.accept(values->get(0));
    }
  }
};
