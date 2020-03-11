#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>

#include "../test_utils.cpp"
#include "../../src/dataframe/row.h"
#include "../../src/dataframe/print_fielder.h"

void basic_row()
{
    // Set up schema
    String *str = new String("First");
    String *str2 = new String("Second");
    String *str3 = new String("One");
    String *str4 = new String("Two");
    Schema *schema = new Schema();
    // Add two cols
    schema->add_column('I', str);
    schema->add_column('F', str2);
    schema->add_column('B', str);
    schema->add_column('S', str2);
    // Add a rows
    schema->add_row(str3);

    // Create row with schema
    Row *row = new Row(*schema);
    GT_EQUALS(row->width(), 4);
    GT_EQUALS(row->get_idx(), 0);

    int i = 35;
    float f = 4.5;
    bool b = true;
    String *s = new String("Hello");
    // Set values in row
    row->set(0, i);
    row->set(1, f);
    row->set(2, b);
    row->set(3, s);
    // Verify values
    GT_EQUALS(row->width(), 4);
    GT_EQUALS(row->get_int(0), 35);
    GT_EQUALS(row->get_float(1), f);
    GT_EQUALS(row->get_bool(2), b);
    String *hello = new String("Hello");
    GT_TRUE(row->get_string(3)->equals(hello));

    // Set index of this row
    row->set_idx(4);
    GT_EQUALS(row->get_idx(), 4);

    // Verify col
    GT_EQUALS(row->col_type(0), 'I');
    GT_EQUALS(row->col_type(1), 'F');
    GT_EQUALS(row->col_type(2), 'B');
    GT_EQUALS(row->col_type(3), 'S');

    PrintFielder *fielder = new PrintFielder();
    row->visit(0, *fielder);

    // make empty row and delete
    Row *row2 = new Row(*schema);

    delete row;
    delete s;
    delete fielder;
    delete row2; 
    delete str;
    delete str2;
    delete str3;
    delete str4;
    delete hello;
    delete schema;
    exit(0);
}

TEST(W1, basic_row)
{
    ASSERT_EXIT_ZERO(basic_row);
}