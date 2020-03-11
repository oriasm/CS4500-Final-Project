#include "gtest/gtest.h"
#include <stdlib.h>
#include <stdio.h>

#include "../test_utils.cpp"
#include "../../src/dataframe/schema.h"

void create_schema_from_types()
{
    // Create schema with char* of types
    Schema *schema = new Schema("IISFB");
    
    // check size
    GT_EQUALS(schema->length(), 0);
    GT_EQUALS(schema->width(), 5);
    GT_EQUALS(schema->col_type(0), 'I');
    GT_EQUALS(schema->col_type(1), 'I');
    GT_EQUALS(schema->col_type(2), 'S');
    GT_EQUALS(schema->col_type(3), 'F');
    GT_EQUALS(schema->col_type(4), 'B');
    String *empty = new String("");
    GT_TRUE(schema->col_name(0)->equals(empty));
    delete empty;
    delete schema;
    exit(0);
}

TEST(W1, create_schema_from_types) {
    ASSERT_EXIT_ZERO(create_schema_from_types);
}

void make_copy_schema()
{
    String *str = new String("First");
    String *str2 = new String("Second");
    String *str3 = new String("One");
    String *str4 = new String("Two");
    Schema *schema = new Schema();
    schema->add_column('I', str);
    schema->add_row(str3);

    // Create schema as copy of existing schema
    Schema *schema2 = new Schema(*schema);
    GT_EQUALS(schema2->length(), 1);
    GT_EQUALS(schema2->width(), 1);
    // Check content
    GT_EQUALS(schema2->col_type(0), 'I');
    // Add new column and row
    schema2->add_column('S', str2);
    schema2->add_row(str4);
    // Check size
    GT_EQUALS(schema2->length(), 2);
    GT_EQUALS(schema2->width(), 2);
    // Check content
    GT_EQUALS(schema2->col_type(1), 'S');
    GT_TRUE(schema2->col_name(1)->equals(str2));
    GT_TRUE(schema2->row_name(1)->equals(str4));
    GT_EQUALS(schema2->col_idx("Second"), 1);
    GT_EQUALS(schema2->row_idx("Two"), 1);

    delete str;
    delete str2;
    delete str3;
    delete str4;
    delete schema;
    delete schema2;
    exit(0);
}

TEST(W1, make_copy_schema) {
    ASSERT_EXIT_ZERO(make_copy_schema);
}

void make_empty_schema()
{
    // Create empty schema, check size
    Schema *schema = new Schema();
    GT_EQUALS(schema->length(), 0);
    GT_EQUALS(schema->width(), 0);

    String *str = new String("First");
    String *str2 = new String("Second");

    // Add two cols
    schema->add_column('I', str);
    schema->add_column('F', str2);
    // check size
    GT_EQUALS(schema->length(), 0);
    GT_EQUALS(schema->width(), 2);
    // Check content
    GT_EQUALS(schema->col_type(0), 'I');
    GT_EQUALS(schema->col_type(1), 'F');
    GT_TRUE(schema->col_name(0)->equals(str));
    GT_EQUALS(schema->col_idx("Second"), 1);

    String *str3 = new String("One");
    String *str4 = new String("Two");

    // Add two rows
    schema->add_row(str3);
    schema->add_row(str4);
    // Check size
    GT_EQUALS(schema->width(), 2);
    GT_EQUALS(schema->length(), 2);
    //Check content
    GT_TRUE(schema->row_name(0)->equals(str3));
    GT_EQUALS(schema->row_idx("Two"), 1);

    delete str;
    delete str2;
    delete str3;
    delete str4;
    delete schema;
    exit(0);
}

TEST(W1, make_empty_schema) {
    ASSERT_EXIT_ZERO(make_empty_schema);
}

void add_columns_to_schema()
{
    // Create empty schema, check size
    Schema *schema = new Schema();
    GT_EQUALS(schema->length(), 0);
    GT_EQUALS(schema->width(), 0);

    String *str = new String("a row");
    String *str2 = new String("another");

    // Add two cols
    schema->add_column('I', str);
    schema->add_column('F', str2);    
    schema->add_column('B', str);
    schema->add_column('S', str2);
    // check size
    GT_EQUALS(schema->length(), 0);
    GT_EQUALS(schema->width(), 4);
    // Check content
    GT_EQUALS(schema->col_type(0), 'I');
    GT_EQUALS(schema->col_type(1), 'F');    
    GT_EQUALS(schema->col_type(2), 'B');
    GT_EQUALS(schema->col_type(3), 'S');
    GT_TRUE(schema->col_name(0)->equals(str));
    GT_TRUE(schema->col_name(2)->equals(str));
    // Should find the first instance of column 'another'
    GT_EQUALS(schema->col_idx("another"), 1);

    String *str3 = new String("One");
    String *str4 = new String("Two");

    // Add two rows
    schema->add_row(str3);
    schema->add_row(str4);
    // Check size
    GT_EQUALS(schema->width(), 4);
    GT_EQUALS(schema->length(), 2);
    //Check content
    GT_TRUE(schema->row_name(0)->equals(str3));
    GT_EQUALS(schema->row_idx("Two"), 1);

    delete str;
    delete str2;
    delete str3;
    delete str4;
    delete schema;
    exit(0);
}

TEST(W1, add_columns_to_schema) {
    ASSERT_EXIT_ZERO(add_columns_to_schema);
}