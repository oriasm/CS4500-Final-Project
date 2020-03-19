//lang::CwC
#pragma once
#include <thread>
#include "column.h"
#include "rower.h"
#include "print_fielder.h"
#include "../kvstore/key.h"
#include "../kvstore/kv_store.h"

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object
{
  // Df's schema
  Schema *schema_;
  // An Array of Columns
  ObjectArray *columns;

public:
  /** Create a data frame with the same columns as the given df but no rows */
  DataFrame(DataFrame &df) : DataFrame(df.get_schema()) {}

  /** Create a data frame from a schema and columns. All columns are created
    * empty. */
  DataFrame(Schema &schema)
  {
    schema_ = new Schema();
    columns = new ObjectArray();
    size_t width = schema.width();
    // Iterate through each column
    for (size_t index = 0; index < width; index++)
    {
      // Add each type and name to this df's schema
      char type = schema.col_type(index);
      String *name = schema.col_name(index);
      schema_->add_column(type, name);

      switch (type)
      {
      case 'B':
      {
        BoolColumn *bools = new BoolColumn();
        columns->push(bools);
        break;
      }
      case 'I':
      {
        IntColumn *ints = new IntColumn();
        columns->push(ints);
        break;
      }
      case 'F':
      {
        FloatColumn *floats = new FloatColumn();
        columns->push(floats);
        break;
      }
      case 'S':
      {
        StringColumn *strings = new StringColumn();
        columns->push(strings);
      }
      default:
        break;
      }
    }
  }

  /* Removes a Dataframe and its contents from memory */
  ~DataFrame()
  {
    delete schema_;

    // iterate through array of column objects
    for (size_t index = 0; index < columns->length(); index++)
    {
      Object *col_ptr = columns->get(index);
      Column *column = dynamic_cast<Column *>(col_ptr);
      delete column;
    }
    delete columns;
  }

  /** Returns the dataframe's schema. Modifying the schema after a dataframe
    * has been created in undefined. */
  Schema &get_schema()
  {
    return *schema_;
  }

  /** Adds a column this dataframe, updates the schema, the new column
    * is external, and appears as the last column of the dataframe, the
    * name is optional and external. A nullptr colum is undefined. */
  void add_column(Column *col, String *name)
  {
    // Column cannot be null
    assert(col);
    // Add type to schema
    char type = col->get_type();
    // Add col to array of cols
    schema_->add_column(type, name);
    columns->push(col);
  }

  /** Used to internally to get a Column from an ObjectArray as a Column
   * 
   */
  Column *get_col_from_array(size_t col)
  {
    // Get column
    Object *current = columns->get(col);
    Column *current_col = dynamic_cast<Column *>(current);
    assert(current_col);
    return current_col;
  }

  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  int get_int(size_t col, size_t row)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(row < current_col->size());

    // Method must be called on correct col type
    IntColumn *int_col = current_col->as_int();
    assert(int_col);

    return int_col->get(row);
  }

  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  bool get_bool(size_t col, size_t row)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(row < current_col->size());

    // Method must be called on correct col type
    BoolColumn *bool_column = current_col->as_bool();
    assert(bool_column);

    return bool_column->get(row);
  }

  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  float get_float(size_t col, size_t row)
  {
    // Get column
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(col < columns->length());
    assert(row < current_col->size());

    // Method must be called on correct col type
    FloatColumn *float_column = current_col->as_float();
    assert(float_column);

    return float_column->get(row);
  }

  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  String *get_string(size_t col, size_t row)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(col < columns->length());
    assert(row < current_col->size());

    // Method must be called on correct col type
    StringColumn *string_column = current_col->as_string();
    assert(string_column);

    return string_column->get(row);
  }

  /** Return the offset of the given column name or -1 if no such col. */
  int get_col(String &col)
  {
    size_t width = schema_->width();
    // Iterate through schema, checking each column name
    for (size_t index = 0; size_t(index) < width; index++)
    {
      String *curr = schema_->col_name(index);
      if (col.equals(curr))
      {
        return index;
      }
    }
    return -1;
  }

  /** Return the offset of the given row name or -1 if no such row. */
  int get_row(String &col)
  {
    size_t length = schema_->length();
    // Iterate through schema, checking each row name
    for (size_t index = 0; size_t(index) < length; index++)
    {
      String *curr = schema_->row_name(index);
      if (col.equals(curr))
      {
        return index;
      }
    }

    return -1;
  }

  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set(size_t col, size_t row, int val)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(row < current_col->size());

    // Method must be called on correct col type
    IntColumn *int_col = current_col->as_int();
    assert(int_col);

    int_col->set(row, val);
  }

  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set(size_t col, size_t row, bool val)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(row < current_col->size());

    // Method must be called on correct col type
    BoolColumn *bool_column = current_col->as_bool();
    assert(bool_column);

    bool_column->set(row, val);
  }

  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set(size_t col, size_t row, float val)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(row < current_col->size());

    // Method must be called on correct col type
    FloatColumn *float_column = current_col->as_float();
    assert(float_column);

    float_column->set(row, val);
  }

  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set(size_t col, size_t row, String *val)
  {
    // Get column
    assert(col < columns->length());
    Column *current_col = get_col_from_array(col);

    // Method must be called on correct indices
    assert(col < columns->length());
    assert(row < current_col->size());

    // Method must be called on correct col type
    StringColumn *string_column = current_col->as_string();
    assert(string_column);

    string_column->set(row, val);
  }

  /** Set the fields of the given row object with values from the columns at
    * the given offset.  If the row is not form the same schema as the
    * dataframe, results are undefined.
    */
  void fill_row(size_t idx, Row &row)
  {
    // row index is valid
    assert(idx < schema_->length());
    // df schema and row schema are of the same length
    assert(schema_->width() == row.width());
    size_t width = schema_->width();
    for (size_t col_index = 0; col_index < width; col_index++)
    {
      Column *column = get_col_from_array(col_index);
      // Column in df and in row are of the same type
      assert(column->get_type() == row.col_type(col_index));

      if (BoolColumn *b = column->as_bool())
      {
        bool val = b->get(idx);
        row.set(col_index, val);
      }
      else if (IntColumn *i = column->as_int())
      {
        int val = i->get(idx);
        row.set(col_index, val);
      }
      else if (FloatColumn *f = column->as_float())
      {
        float val = f->get(idx);
        row.set(col_index, val);
      }
      else if (StringColumn *s = column->as_string())
      {
        String *val = s->get(idx);
        row.set(col_index, val);
      }
    }
  }

  /** Add a row at the end of this dataframe. The row is expected to have
   *  the right schema and be filled with values, otherwise undedined.  */
  void add_row(Row &row)
  {
    // Add a row to the schema with no name
    String *empty = new String("");
    schema_->add_row(empty);
    delete empty;

    size_t width = row.width();
    // Row has same number of cols as schema
    assert(schema_->width() == width);

    for (size_t index = 0; index < width; index++)
    {
      Column *column = get_col_from_array(index);

      if (BoolColumn *b = column->as_bool())
      {
        bool val = row.get_bool(index);
        b->push_back(val);
      }
      else if (IntColumn *i = column->as_int())
      {
        int val = row.get_int(index);
        i->push_back(val);
      }
      else if (FloatColumn *f = column->as_float())
      {
        float val = row.get_float(index);
        f->push_back(val);
      }
      else if (StringColumn *s = column->as_string())
      {
        String *val = row.get_string(index);
        s->push_back(val);
      }
    }
  }

  /** The number of rows in the dataframe. */
  size_t nrows()
  {
    return schema_->length();
  }

  /** The number of columns in the dataframe.*/
  size_t ncols()
  {
    return schema_->width();
  }

  /** Visit rows in order */
  void map(Rower &r)
  {
    size_t length = nrows();
    for (size_t index = 0; index < length; index++)
    {
      // Get the row object
      Row *row = create_one_row(index);

      // Rower returns true on success
      assert(r.accept(*row));
      delete row;
    }
    r.join_delete(nullptr);
  }

  /** Visit rows in order using multiple threads */
  void pmap(Rower &r)
  {
    size_t threads = 4;
    size_t length = this->nrows();
    size_t to_read = length / threads;

    std::thread *pool[threads];
    Rower **clones = new Rower *[threads];

    for (size_t i = 0; i < threads; i++)
    {
      size_t end_index;
      if (i == threads - 1)
      {
        end_index = length;
      }
      else
      {
        end_index = (i + 1) * to_read;
      }

      Rower *clone = r.clone();
      clones[i] = clone;
      pool[i] = new std::thread(&DataFrame::index_map, this, std::ref(*clone), i * to_read, end_index);
    }
    for (size_t i = 0; i < threads; i++)
    {
      pool[i]->join();
      r.join_delete(clones[i]);
    }
  }

  /** Maps over certain indices for a DataFrame */
  void index_map(Rower &r, size_t start, size_t end)
  {
    // Iterates from start to end
    for (size_t i = start; i < end; i++)
    {
      // Gets an individual row
      Row *row = create_one_row(i);
      r.accept(*row);
      delete row;
    }
  }

  /** Create a new dataframe, constructed from rows for which the given Rower
    * returned true from its accept method. */
  DataFrame *filter(Rower &r)
  {
    // New df
    DataFrame *filtered = new DataFrame(*schema_);
    size_t length = nrows();
    for (size_t index = 0; index < length; index++)
    {
      // Get the row object
      Row *row = create_one_row(index);
      // Rower accepts row
      bool keep = r.accept(*row);
      delete row;

      // if rower return true, copy row into new df
      if (keep)
      {
        // Create a blank row with this df's schema
        Row *copy = new Row(*schema_);
        // Fill it with values from this df
        fill_row(index, *copy);
        filtered->add_row(*copy);
        delete copy;
      }
    }

    return filtered;
  }

  // Creates a row object at the given index
  Row *create_one_row(size_t index)
  {
    size_t width = ncols();
    Row *curr = new Row(*schema_);
    for (size_t col_index = 0; col_index < width; col_index++)
    {
      Column *column = get_col_from_array(col_index);

      if (BoolColumn *b = column->as_bool())
      {
        bool val = b->get(index);
        curr->set(col_index, val);
      }
      else if (IntColumn *i = column->as_int())
      {
        int val = i->get(index);
        curr->set(col_index, val);
      }
      else if (FloatColumn *f = column->as_float())
      {
        float val = f->get(index);
        curr->set(col_index, val);
      }
      else if (StringColumn *s = column->as_string())
      {
        String *val = s->get(index);
        curr->set(col_index, val);
      }
    }

    assert(curr);
    curr->set_idx(index);
    return curr;
  }

  /** Serializes this Dataframe into a char array */
  char *serialize() {
    StrBuff *serial = new StrBuff();
    // add type to serial
    serial->c("{Dataframe,");
    // serialize schema
    char *schema_serial = schema_->serialize();
    serial->c("schema:");
    serial->c(schema_serial);
    // serialize array of columns
    char * columns_serial = columns->serialize();
    serial->c(", columns:");
    serial->c(columns_serial);

    // close dataframe bracket
    serial->c("}");
    return Serial::extract_char_(serial);
  }

  /** Print the dataframe in SoR format to standard output. */
  void print()
  {
    size_t length = nrows();
    for (size_t index = 0; index < length; index++)
    {
      Row *row = create_one_row(index);
      // Create PrintFielder Object
      PrintFielder *printer = new PrintFielder();
      row->visit(index, *printer);
      delete printer;
      delete row;
    }
  }

  /** Creates a schema with the given number of columns of the given type */
  static Schema *createSchema(size_t size, char typ)
  {
    Schema *sch = new Schema();
    String *empty_name = new String("");

    for (size_t index = 0; index < size; index++)
    {
      sch->add_column(typ, empty_name);
    }

    delete empty_name;
    return sch;
  }

  /** Creates a new Dataframe containing one row with all the values in the given array,
   *  saves it in the given kvstore with the given key
   */
  static DataFrame *fromArray(Key &key, KVStore &store, size_t size, float *values)
  {
    // create schema with 'size' number of floats
    Schema *sch = createSchema(size, 'F');

    // create dataframe with that schema
    DataFrame *df = new DataFrame(*sch);

    // create a single row with all given values
    Row *row = new Row(*sch);
    for (size_t index = 0; index < size; index++)
    {
      row->set(index, values[index]);
    }

    // add that row to df
    df->add_row(*row);

    delete sch;
    delete row;

    // save df to key and in kvstore
    Value *df_val = new Value(df);
    store.put(key, df_val);

    delete df_val;
    return df;
  }
};
