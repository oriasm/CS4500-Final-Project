//lang::CwC
#pragma once
#include "string.h"
#include <cassert>
#include <cstdio>

size_t MAX_ARRAY_SIZE = 2048;          // Constant for the maximum nested array size
size_t STARTING_DATA_ARRAY_SIZE = 256; // Constant for the starting point for arrays of pointers

/****************************************************************************
 * Array::
 *
 * Stores an array of Objects. Does not own its elements.
 * To ensure data is not copied, data is stored with an array of array of Object.
 * THIS SHOULD NEVER BE USED, ONLY USE SUBCLASSES.
 */
class Array : public Object
{
public:
  size_t size_;         /* Maximum size of an individual nested array */
  size_t total_length_; /* Current total length of the Array */
  size_t data_size_;    /* Maximum size of the array of Object pointers */
  size_t data_length_;  /* Current length of the current nested array */
  Object ***data_;      /* Actual data stored within the Array, an array of Object pointers */

  /* Constructs an empty Array, initially can hold 1024 elements */
  Array()
  {
    size_ = MAX_ARRAY_SIZE;
    total_length_ = 0;
    data_size_ = STARTING_DATA_ARRAY_SIZE;
    data_length_ = 0;
  }

  /* Removes an Array from memory */
  virtual ~Array() {}

  /* Returns the number of elements in an Array */
  virtual size_t length()
  {
    return total_length_;
  }

  /* Grows the Array if needed */
  virtual void grow_(size_t new_length) {}

  /* Adds an Object to the end of the Array */
  void push(Object *o) {}

  /* Returns an Object at a given index */
  Object *get(size_t idx)
  {
    return nullptr;
  }

  /**
     * Adds an Object at a given index
     * Returns overwritten Object */
  Object *set(size_t idx, Object *o)
  {
    return nullptr;
  }

  /* Checks if this Array is equal to a given Object */
  virtual bool equals(Object *o)
  {
    Array *casted_arr = dynamic_cast<Array *>(o);
    if (casted_arr)
    {
      return casted_arr->hash() == hash();
    }

    return false;
  }

  /* Generates a hash value for this Array */
  virtual size_t hash()
  {
    return reinterpret_cast<size_t>(this);
  }
};

/****************************************************************************
 * ObjectArray::
 *
 * Stores an array of Objects. Does not own its elements.
 * To ensure data is not copied, data is stored with an array of array of Object.
 */
class ObjectArray : public Object
{
public:
  size_t size_;         /* Maximum size of an individual nested array */
  size_t total_length_; /* Current total length of the Array */
  size_t data_size_;    /* Maximum size of the array of Object pointers */
  size_t data_length_;  /* Current length of the current nested array */
  Object ***data_;      /* Actual data stored within the Array, an array of Object pointers */

  /* Constructs an empty Array, initially can hold 1024 elements */
  ObjectArray()
  {
    size_ = MAX_ARRAY_SIZE;
    total_length_ = 0;
    data_size_ = STARTING_DATA_ARRAY_SIZE;
    data_length_ = 0;
    data_ = new Object **[data_size_];
  }

  /* Removes an Array from memory */
  virtual ~ObjectArray()
  {
    // Remove all nested arrays
    for (size_t i = 0; i < data_length_; i++)
    {
      delete[] data_[i];
    }
    // Remove main array
    delete[] data_;
  }

  /* Returns the number of elements in an Array */
  virtual size_t length()
  {
    return total_length_;
  }

  /* Grows the Array if needed */
  virtual void grow_(size_t new_length)
  {
    // There is still room in our current array
    if (new_length < size_)
    {
      return;
    }
    // There is still room in our main data store
    if (data_length_ + 1 < data_size_)
    {
      return;
    }

    // Copy all array pointers to new array
    data_size_ = data_size_ * 2;
    Object ***new_data = new Object **[data_size_];
    for (size_t i = 0; i < data_length_; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }

  /* Adds an Object to the end of the Array */
  void push(Object *o)
  {
    size_t insert_idx = total_length_ % size_;
    grow_(insert_idx + 1);
    if (insert_idx == 0)
    {
      data_[data_length_] = new Object *[size_];
      data_length_++;
    }
    Object **current_arr = data_[data_length_ - 1];
    current_arr[insert_idx] = o;
    total_length_++;
  }

  /* Returns an Object at a given index */
  Object *get(size_t idx)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    Object **current_arr = data_[data_idx];
    return current_arr[nested_idx];
  }

  /**
     * Adds an Object at a given index
     * Returns overwritten Object */
  Object *set(size_t idx, Object *o)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    Object **current_arr = data_[data_idx];
    Object *overwritten = current_arr[nested_idx];
    current_arr[nested_idx] = o;
    return overwritten;
  }

  /* Checks if this Array is equal to a given Object */
  virtual bool equals(Object *o)
  {
    Array *casted_arr = dynamic_cast<Array *>(o);
    if (casted_arr)
    {
      return casted_arr->hash() == hash();
    }

    return false;
  }

  /* Generates a hash value for this Array */
  virtual size_t hash()
  {
    size_t hash = 0;
    size_t len_counter = 0;
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        if (len_counter >= total_length_)
        {
          break;
        }
        hash = hash * 31 + data_[i][j]->hash();
        len_counter++;
      }
    }
    return hash;
  }
};

/****************************************************************************
 * BoolArray::
 *
 * Stores an array of bool. Does not own its elements.
 * To ensure data is not copied, data is stored with an array of array of bool.
 */
class BoolArray : public Array
{
public:
  bool **data_; /* Actual data stored within the Array, an array of bool pointers */

  /* Constructs an empty BoolArray, initially can hold 1024 elements */
  BoolArray()
  {
    size_ = MAX_ARRAY_SIZE;
    total_length_ = 0;
    data_size_ = STARTING_DATA_ARRAY_SIZE;
    data_length_ = 0;
    data_ = new bool *[data_size_];
  }

  /* Removes a BoolArray from memory */
  virtual ~BoolArray()
  {
    // Remove all nested arrays
    for (size_t i = 0; i < data_length_; i++)
    {
      delete[] data_[i];
    }
    // Remove main array
    delete[] data_;
  }

  /* Grows the BoolArray if needed */
  virtual void grow_(size_t new_length)
  {
    // There is still room in our current array
    if (new_length < size_)
    {
      return;
    }
    // There is still room in our main data store
    if (data_length_ + 1 < data_size_)
    {
      return;
    }

    // Copy all array pointers to new array
    data_size_ = data_size_ * 2;
    bool **new_data = new bool *[data_size_];
    for (size_t i = 0; i < data_length_; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }

  /* Adds a bool to the end of the Array */
  virtual void push(bool b)
  {
    size_t insert_idx = total_length_ % size_;
    grow_(insert_idx + 1);
    if (insert_idx == 0)
    {
      data_[data_length_] = new bool[size_];
      data_length_++;
    }
    bool *current_arr = data_[data_length_ - 1];
    current_arr[insert_idx] = b;
    total_length_++;
  }

  /* Returns a bool at a given index */
  virtual bool set(size_t idx, bool b)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    bool *current_arr = data_[data_idx];
    bool overwritten = current_arr[nested_idx];
    current_arr[nested_idx] = b;
    return overwritten;
  }

  /**
     * Adds a bool at a given index
     * Returns overwritten bool */
  bool get(size_t idx)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    bool *current_arr = data_[data_idx];
    return current_arr[nested_idx];
  }

  /* Checks if this Array is equal to a given Object */
  virtual bool equals(Object *o)
  {
    BoolArray *casted_arr = dynamic_cast<BoolArray *>(o);
    if (casted_arr)
    {
      return casted_arr->hash() == hash();
    }

    return false;
  }

  /* Calculates the hash for a BoolArray */
  virtual size_t hash()
  {
    size_t hash = 1;
    size_t len_counter = 0;
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        if (len_counter >= total_length_)
        {
          break;
        }
        hash = hash * 31 + data_[i][j];
        len_counter++;
      }
    }
    return hash;
  }

  void print_()
  {
    size_t len_counter = 0;
    printf("[");
    for (size_t i = 0; i < data_length_; i++)
    {
      bool *current_arr = data_[i];
      for (size_t j = 0; j < size_; j++)
      {
        if (current_arr[j] == 0)
        {
          printf("false");
        }
        else
        {
          printf("true");
        }
        len_counter++;
        if (len_counter >= total_length_)
        {
          break;
        }
        else
        {
          printf(", ");
        }
      }
    }
    printf("]\n");
  }
};

/****************************************************************************
 * FloatArray::
 *
 * Stores an array of float. Does not own its elements.
 * Data is stored in an array of array of float.
 */
class FloatArray : public Array
{
public:
  float **data_; /* Actual data stored within the Array, an array of float pointers */

  /* Constructs an empty FloatArray, initially can hold 1024 elements */
  FloatArray()
  {
    size_ = MAX_ARRAY_SIZE;
    total_length_ = 0;
    data_size_ = STARTING_DATA_ARRAY_SIZE;
    data_length_ = 0;
    data_ = new float *[data_size_];
  }

  /* Removes a FloatArray from memory */
  virtual ~FloatArray()
  {
    // Remove all nested arrays
    for (size_t i = 0; i < data_length_; i++)
    {
      delete[] data_[i];
    }
    // Remove main array
    delete[] data_;
  }

  /* Grows the FloatArray if needed */
  virtual void grow_(size_t new_length)
  {
    // There is still room in our current array
    if (new_length < size_)
    {
      return;
    }
    // There is still room in our main data store
    if (data_length_ + 1 < data_size_)
    {
      return;
    }

    // Copy all array pointers to new array
    data_size_ = data_size_ * 2;
    float **new_data = new float *[data_size_];
    for (size_t i = 0; i < data_length_; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }

  /* Adds a float to the end of the Array */
  virtual void push(float f)
  {
    size_t insert_idx = total_length_ % size_;
    grow_(insert_idx + 1);
    if (insert_idx == 0)
    {
      data_[data_length_] = new float[size_];
      data_length_++;
    }
    float *current_arr = data_[data_length_ - 1];
    current_arr[insert_idx] = f;
    total_length_++;
  }

  /* Returns a float at a given index */
  virtual float set(size_t idx, float f)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    float *current_arr = data_[data_idx];
    float overwritten = current_arr[nested_idx];
    current_arr[nested_idx] = f;
    return overwritten;
  }

  /**
     * Adds a float at a given index
     * Returns overwritten float */
  float get(size_t idx)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    float *current_arr = data_[data_idx];
    return current_arr[nested_idx];
  }

  /* Checks if this Array is equal to a given Object */
  virtual bool equals(Object *o)
  {
    FloatArray *casted_arr = dynamic_cast<FloatArray *>(o);
    if (casted_arr)
    {
      return casted_arr->hash() == hash();
    }

    return false;
  }

  /* Calculates the hash for a FloatArray */
  virtual size_t hash()
  {
    size_t hash = 1;
    size_t len_counter = 0;
    for (size_t i = 0; i < data_length_; i++)
    {
      float *current_arr = data_[i];
      for (size_t j = 0; j < size_; j++)
      {
        if (len_counter >= total_length_)
        {
          break;
        }
        hash = hash * 31 + current_arr[j];
        len_counter++;
      }
    }
    return hash;
  }

  void print_()
  {
    size_t len_counter = 0;
    printf("[");
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        printf("%f", data_[i][j]);
        len_counter++;
        if (len_counter >= total_length_)
        {
          break;
        }
        else
        {
          printf(", ");
        }
      }
    }
    printf("]\n");
  }
};

/****************************************************************************
 * IntArray::
 *
 * Stores an array of int. Does not own its elements.
 * To ensure data is not copied, data is stored with an array of array of int.
 */
class IntArray : public Array
{
public:
  int **data_; /* Actual data stored within the Array, an array of int pointers */

  /* Constructs an empty IntArray, initially can hold 1024 elements */
  IntArray()
  {
    size_ = MAX_ARRAY_SIZE;
    total_length_ = 0;
    data_size_ = STARTING_DATA_ARRAY_SIZE;
    data_length_ = 0;
    data_ = new int *[data_size_];
  }

  /* Removes a IntArray from memory */
  virtual ~IntArray()
  {
    // Remove all nested arrays
    for (size_t i = 0; i < data_length_; i++)
    {
      delete[] data_[i];
    }
    // Remove main array
    delete[] data_;
  }

  /* Grows the IntArray if needed */
  virtual void grow_(size_t new_length)
  {
    // There is still room in our current array
    if (new_length < size_)
    {
      return;
    }
    // There is still room in our main data store
    if (data_length_ + 1 < data_size_)
    {
      return;
    }

    // Copy all array pointers to new array
    data_size_ = data_size_ * 2;
    int **new_data = new int *[data_size_];
    for (size_t i = 0; i < data_length_; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }

  /* Adds an int to the end of the Array */
  virtual void push(int i)
  {
    size_t insert_idx = total_length_ % size_;
    grow_(insert_idx + 1);
    if (insert_idx == 0)
    {
      data_[data_length_] = new int[size_];
      data_length_++;
    }
    int *current_arr = data_[data_length_ - 1];
    current_arr[insert_idx] = i;
    total_length_++;
  }

  /* Returns an int at a given index */
  int set(size_t idx, int i)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    int *current_arr = data_[data_idx];
    int overwritten = current_arr[nested_idx];
    current_arr[nested_idx] = i;
    return overwritten;
  }

  /**
     * Adds a int at a given index
     * Returns overwritten int */
  int get(size_t idx)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    int *current_arr = data_[data_idx];
    return current_arr[nested_idx];
  }

  /* Checks if this Array is equal to a given Object */
  virtual bool equals(Object *o)
  {
    IntArray *casted_arr = dynamic_cast<IntArray *>(o);
    if (casted_arr)
    {
      return casted_arr->hash() == hash();
    }

    return false;
  }

  /* Calculates the hash for a IntArray */
  virtual size_t hash()
  {
    size_t hash = 1;
    size_t len_counter = 0;
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        if (len_counter >= total_length_)
        {
          break;
        }
        hash = hash * 31 + data_[i][j];
        len_counter++;
      }
    }
    return hash;
  }

  /* Simple print function, prints all contents */
  void print_()
  {
    size_t len_counter = 0;
    printf("[");
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        printf("%d", data_[i][j]);
        len_counter++;
        if (len_counter >= total_length_)
        {
          break;
        }
        else
        {
          printf(", ");
        }
      }
    }
    printf("]\n");
  }
};

/****************************************************************************
 * StringArray::
 *
 * Stores an array of String. Does not own its elements.
 * To ensure data is not copied, data is stored with an array of array of String.
 */
class StringArray : public Array
{
public:
  String ***data_; /* Actual data stored within the Array, an array of String pointers */

  /* Constructs an empty StringArray, initially can hold 1024 elements */
  StringArray()
  {
    size_ = MAX_ARRAY_SIZE;
    total_length_ = 0;
    data_size_ = STARTING_DATA_ARRAY_SIZE;
    data_length_ = 0;
    data_ = new String **[data_size_];
  }

  /* Removes a StringArray from memory */
  virtual ~StringArray()
  {
    // Remove all nested arrays
    for (size_t i = 0; i < data_length_; i++)
    {
      delete[] data_[i];
    }
    // Remove main array
    delete[] data_;
  }

  /* Grows the StringArray if needed */
  virtual void grow_(size_t new_length)
  {
    // There is still room in our current array
    if (new_length < size_)
    {
      return;
    }
    // There is still room in our main data store
    if (data_length_ + 1 < data_size_)
    {
      return;
    }

    // Copy all array pointers to new array
    data_size_ = data_size_ * 2;
    String ***new_data = new String **[data_size_];
    for (size_t i = 0; i < data_length_; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }

  /* Adds a String to the end of the Array */
  virtual void push(String *str)
  {
    size_t insert_idx = total_length_ % size_;
    grow_(insert_idx + 1);
    if (insert_idx == 0)
    {
      data_[data_length_] = new String *[size_];
      data_length_++;
    }
    String **current_arr = data_[data_length_ - 1];
    current_arr[insert_idx] = str;
    total_length_++;
  }

  /* Returns a String at a given index */
  String *set(size_t idx, String *str)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    String **current_arr = data_[data_idx];
    String *overwritten = current_arr[nested_idx];
    current_arr[nested_idx] = str;
    return overwritten;
  }

  /**
     * Adds a String at a given index
     * Returns overwritten String */
  String *get(size_t idx)
  {
    size_t nested_idx = idx % size_;
    size_t data_idx = idx / size_;
    String **current_arr = data_[data_idx];
    return current_arr[nested_idx];
  }

  /* Checks if this Array is equal to a given Object */
  virtual bool equals(Object *o)
  {
    StringArray *casted_arr = dynamic_cast<StringArray *>(o);
    if (casted_arr)
    {
      return casted_arr->hash() == hash();
    }

    return false;
  }

  /* Calculates the hash for a StringArray */
  virtual size_t hash()
  {
    size_t hash = 1;
    size_t len_counter = 0;
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        if (len_counter >= total_length_)
        {
          break;
        }
        hash = hash * 31 + data_[i][j]->hash();
        len_counter++;
      }
    }
    return hash;
  }

  /* Simple print function, prints all contents */
  void print_()
  {
    size_t len_counter = 0;
    printf("[");
    for (size_t i = 0; i < data_length_; i++)
    {
      for (size_t j = 0; j < size_; j++)
      {
        printf("%s", data_[i][j]->c_str());
        len_counter++;
        if (len_counter >= total_length_)
        {
          break;
        }
        else
        {
          printf(", ");
        }
      }
    }
    printf("]\n");
  }
};