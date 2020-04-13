//lang::CwC
#pragma once
#include "../utilities/array.h"
#include <cstdio>
#include <cstdarg>
#include <cassert>

class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;

/* Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class Column : public Object
{
public:
  /** Type converters: Return the same column under its actual type, or
   *  nullptr if of the wrong type.  */
  virtual IntColumn *as_int()
  {
    return nullptr;
  }
  virtual BoolColumn *as_bool()
  {
    return nullptr;
  }
  virtual FloatColumn *as_float()
  {
    return nullptr;
  }
  virtual StringColumn *as_string()
  {
    return nullptr;
  }

  /** Type appropriate push_back methods. Calling the wrong method is
    * undefined behavior. **/
  virtual void push_back(int val)
  {
    perror("Cannot add an int to this column type.");
  }
  virtual void push_back(bool val)
  {
    perror("Cannot add a bool to this column type.");
  }
  virtual void push_back(float val)
  {
    perror("Cannot add a float to this column type.");
  }
  virtual void push_back(String *val)
  {
    perror("Cannot add a String to this column type.");
  }

  /** Returns the number of elements in the column. */
  virtual size_t size()
  {
    return 0;
  }

  /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'. */
  char get_type()
  {
    if (as_int() != nullptr)
    {
      return 'I';
    }
    if (as_bool() != nullptr)
    {
      return 'B';
    }
    if (as_float() != nullptr)
    {
      return 'F';
    }
    if (as_string() != nullptr)
    {
      return 'S';
    }
    return '\0';
  }
};

/*************************************************************************
 * BoolColumn::
 * Holds primitive bool values, unwrapped.
 */
class BoolColumn : public Column
{
public:
  BoolArray *data_; // The array of bool stored inside a column

  /* Initializes an empty BoolColumn */
  BoolColumn()
  {
    data_ = new BoolArray();
  }

  /**
   * Takes in a variable number of bool.
   * First argument MUST be the number of booleans passed in.
   * Following arguments are the bool to be added to a Column. */
  BoolColumn(int n, ...)
  {
    data_ = new BoolArray();
    bool val;
    va_list vl;
    va_start(vl, n);
    for (int i = 0; i < n; i++)
    {
      val = va_arg(vl, int);
      data_->push(val);
    }
    va_end(vl);
  }

  /* Clears a BoolColumn from memory, not responsible for contents */
  ~BoolColumn()
  {
    delete data_;
  }

  /* Returns the pointer to this BoolColumn */
  BoolColumn *as_bool()
  {
    return this;
  }

  /* Gets a bool at the given index */
  bool get(size_t idx)
  {
    assert(idx < data_->total_length_); // check index is valid
    return data_->get(idx);
  }

  /* Sets a bool value at the given index */
  void set(size_t idx, bool val)
  {
    assert(idx < data_->total_length_); // check index is valid
    data_->set(idx, val);
  }

  /* Adds a bool to the BoolColumn */
  void push_back(bool val)
  {
    data_->push(val);
  }

  /* Returns the number of elements in the BoolColumn */
  size_t size()
  {
    return data_->length();
  }
};

/*************************************************************************
 * FloatColumn::
 * Holds primitive float values, unwrapped.
 */
class FloatColumn : public Column
{
public:
  FloatArray *data_; // The array of float stored inside a column

  /* Initializes an empty FloatColumn */
  FloatColumn()
  {
    data_ = new FloatArray();
  }

  /**
   * Takes in a variable number of float.
   * First argument MUST be the number of floats passed in.
   * Following arguments are the float to be added to a Column. */
  FloatColumn(int n, ...)
  {
    data_ = new FloatArray();
    float val;
    va_list vl;
    va_start(vl, n);
    for (int i = 0; i < n; i++)
    {
      val = va_arg(vl, double);
      data_->push(val);
    }
    va_end(vl);
  }

  /* Clears a FloatColumn from memory, not responsible for contents */
  ~FloatColumn()
  {
    delete data_;
  }

  /* Returns the pointer to this FloatColumn */
  FloatColumn *as_float()
  {
    return this;
  }

  /* Gets a float at the given index */
  float get(size_t idx)
  {
    assert(idx < data_->total_length_); // check index is valid
    return data_->get(idx);
  }

  /* Sets a float value at the given index */
  void set(size_t idx, float val)
  {
    assert(idx < data_->total_length_); // check index is valid
    data_->set(idx, val);
  }

  /* Adds a float to the FloatColumn */
  void push_back(float val)
  {
    data_->push(val);
  }

  /* Returns the number of elements in the FloatColumn */
  size_t size()
  {
    return data_->length();
  }
};

/*************************************************************************
 * IntColumn::
 * Holds primitive int values, unwrapped.
 */
class IntColumn : public Column
{
public:
  IntArray *data_; // The array of int stored inside a column

  /* Initializes an empty IntColumn */
  IntColumn()
  {
    data_ = new IntArray();
  }

  /**
   * Takes in a variable number of int.
   * First argument MUST be the number of ints passed in.
   * Following arguments are the int to be added to a Column. */
  IntColumn(int n, ...)
  {
    data_ = new IntArray();
    int val;
    va_list vl;
    va_start(vl, n);
    for (int i = 0; i < n; i++)
    {
      val = va_arg(vl, int);
      data_->push(val);
    }
    va_end(vl);
  }

  /* Clears a IntColumn from memory, not responsible for contents */
  ~IntColumn()
  {
    delete data_;
  }

  /* Returns the pointer to this IntColumn */
  IntColumn *as_int()
  {
    return this;
  }

  /* Gets an int at the given index */
  int get(size_t idx)
  {
    assert(idx < data_->total_length_); // check index is valid
    return data_->get(idx);
  }

  /* Sets an int value at the given index */
  void set(size_t idx, int val)
  {
    assert(idx < data_->total_length_); // check index is valid
    data_->set(idx, val);
  }

  /* Adds an int to the IntColumn */
  void push_back(int val)
  {
    data_->push(val);
  }

  /* Returns the number of elements in the IntColumn */
  size_t size()
  {
    return data_->length();
  }
};

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 * Even though Strings are external, copies are made and must be freed
 */
class StringColumn : public Column
{
public:
  StringArray *data_; // The array of String stored inside a column

  /* Initializes an empty StringColumn */
  StringColumn()
  {
    data_ = new StringArray();
  }

  /**
   * Takes in a variable number of int.
   * First argument MUST be the number of ints passed in.
   * Following arguments are the String pointers to be added to a Column. */
  StringColumn(int n, ...)
  {
    data_ = new StringArray();
    String *val;
    va_list vl;
    va_start(vl, n);
    for (int i = 0; i < n; i++)
    {
      val = va_arg(vl, String *);
      String *new_str = new String(*val);
      data_->push(new_str);
    }
    va_end(vl);
  }

  /* Clears a StringColumn from memory, not responsible for contents */
  ~StringColumn()
  {
    for (size_t i = 0; i < data_->total_length_; i++)
    {
      String *to_delete = data_->get(i);
      delete to_delete;
    }
    delete data_;
  }

  /* Returns the pointer to this StringColumn */
  StringColumn *as_string()
  {
    return this;
  }

  /* Returns the string at idx; undefined on invalid idx.*/
  String *get(size_t idx)
  {
    assert(idx < data_->total_length_); // check index is valid
    return data_->get(idx);
  }

  /* Acquire ownership for the string. */
  void set(size_t idx, String *val)
  {
    assert(idx < data_->total_length_); // check index is valid
    String *new_str = new String(val->c_str());
    String *old_str = data_->set(idx, new_str);
    delete old_str;
  }

  /* Adds a String to the StringColumn */
  void push_back(String *val)
  {
    String *new_str = new String(val->c_str());
    data_->push(new_str);
  }

  /* Returns the number of elements in the StringColumn */
  size_t size()
  {
    return data_->length();
  }
};
