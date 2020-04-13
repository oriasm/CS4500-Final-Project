//lang::CwC
#pragma once
#include <cassert>
#include "../utilities/string.h"
#include "../utilities/array.h"

/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object
{
public:
  StrBuff *schema;
  StringArray *col_names;
  StringArray *row_names;
  size_t num_columns;
  size_t num_rows;

  /** Copying constructor */
  Schema(Schema &from) : Schema()
  {

    // Iterate through each char in schema
    for (size_t index = 0; index < from.width(); index++)
    {
      char typ = from.col_type(index);
      String *empty = new String("");
      add_column(typ, empty);
      delete empty;
    }

    for (size_t index = 0; index < from.length(); index++)
    {
      String *empty = new String("");
      add_row(empty);
      delete empty;
    }
  }

  /** Create an empty schema **/
  Schema()
  {
    num_columns = 0;
    num_rows = 0;
    col_names = new StringArray();
    row_names = new StringArray();
    schema = new StrBuff();
  }

  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr is
    * undefined. **/
  Schema(const char *types) : Schema()
  {
    // Check for valid schema
    for (size_t index = 0; index < strlen(types); index++)
    {
      assert(types[index] == 'B' || types[index] == 'F' || types[index] == 'I' || types[index] == 'S');
      String *empty = new String("");
      add_column(types[index], empty);
      delete empty;
    }
  }

  /* Clears Schema and contents from memory */
  ~Schema()
  {
    String *types = schema->get();
    delete types;
    delete schema;

    for (size_t index = 0; index < col_names->length(); index++)
    {
      delete col_names->get(index);
    }

    for (size_t index = 0; index < row_names->length(); index++)
    {
      delete row_names->get(index);
    }

    delete col_names;
    delete row_names;
  }

  /** Add a column of the given type and name (can be nullptr), name
    * is external. */
  void add_column(char typ, String *name)
  {
    char *ptr = new char[2];
    ptr[0] = typ;
    ptr[1] = '\0';
    schema->c(ptr);
    delete[] ptr;

    if (name)
    {
      String *my_name = new String(*name);
      col_names->push(my_name);
    }
    else
    {
      // nullptr = no column name
      col_names->push(name);
    }

    num_columns++;
  }

  /** Add a row with a name (possibly nullptr), name is external. */
  void add_row(String *name)
  {
    if (name)
    {
      String *my_name = new String(*name);
      row_names->push(my_name);
    }
    else
    {
      row_names->push(name);
    }
    num_rows++;
  }

  /** Return name of row at idx; nullptr indicates no name. An idx >= width
    * is undefined. */
  String *row_name(size_t idx)
  {
    assert(idx < num_rows);
    return row_names->get(idx);
  }

  /** Return name of column at idx; nullptr indicates no name given.
    *  An idx >= width is undefined.*/
  String *col_name(size_t idx)
  {
    assert(idx < num_columns);
    return col_names->get(idx);
  }

  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx)
  {
    assert(idx < num_columns);
    return schema->val_[idx];
  }

  /** Given a name and an array to search, returns its index or -1**/
  int search_for_name_(const char *name, StringArray *array)
  {
    // Makes temp string to compare names
    String *search = new String(name, strlen(name));
    for (int index = 0; size_t(index) < array->length(); index++)
    {
      String *current = array->get(index);
      if (search->equals(current))
      {
        delete search;
        return index;
      }
    }

    delete search;
    return -1;
  }

  /** Given a column name return its index, or -1. */
  int col_idx(const char *name)
  {
    return search_for_name_(name, col_names);
  }

  /** Given a row name return its index, or -1. */
  int row_idx(const char *name)
  {
    return search_for_name_(name, row_names);
  }

  /** The number of columns */
  size_t width()
  {
    return num_columns;
  }

  /** The number of rows */
  size_t length()
  {
    return num_rows;
  }

  /** Extracts the char* from a StrBuff
   * Frees the StrBuff, removes a lot of code duplication */
  char *extract_char_schema_(StrBuff *str_buff)
  {
    String *str = str_buff->get();
    delete str_buff;
    char *c = str->c_str();
    delete str;
    return c;
  }

  /** Serializes this schema into a char array */
  char *serialize()
  {
    StrBuff *serial = new StrBuff();
    serial->c("{Schema,");
    // serialize schema string
    char *schema_str = schema->val_;
    serial->c("schema:");
    serial->c(schema_str);

    // serialize num rows
    char *rows = new char[32];
    sprintf(rows, "%zu", num_rows);
    serial->c(",num_rows:");
    serial->c(rows);

    // serialize num columns
    char *columns = new char[32];
    sprintf(columns, "%zu", num_columns);
    serial->c(", num_columns:");
    serial->c(columns);

    delete[] columns;
    delete[] rows;
    return extract_char_schema_(serial);
  }
};
