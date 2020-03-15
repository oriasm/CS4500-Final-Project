#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "../test_utils.cpp"
#include "../../src/dataframe/sum_rower.h"
#include "../../src/dataframe/filter_sum_rower.h"
#include "../../src/dataframe/highest_gpa_rower.h"
#include "../../src/dataframe/dataframe.h"
#include "../../src/dataframe/row.h"

void dataframe_from_schema()
{
    // Create schema with char* of types
    Schema *schema = new Schema("IS");

    // Create dataframe with schema 'IS'
    DataFrame *df = new DataFrame(*schema);
    GT_EQUALS(df->ncols(), 2);
    GT_EQUALS(df->nrows(), 0);

    // Create three rows with above schema
    String *maria = new String("Maria");
    Row *first = new Row(*schema);
    first->set(0, 100);
    first->set(1, maria);
    String *chase = new String("Chase");
    Row *second = new Row(*schema);
    second->set(0, 200);
    second->set(1, chase);
    String *bob = new String("Bob");
    Row *third = new Row(*schema);
    third->set(0, 300);
    third->set(1, bob);

    // Add rows to df
    df->add_row(*first);
    df->add_row(*second);
    df->add_row(*third);

    // Verify length
    GT_EQUALS(df->nrows(), 3);
    // verify first column
    GT_EQUALS(df->get_int(0, 0), 100);
    GT_EQUALS(df->get_int(0, 1), 200);
    GT_EQUALS(df->get_int(0, 2), 300);
    // Create new strings for comparison
    String *val1 = new String("Maria");
    String *val2 = new String("Chase");
    String *val3 = new String("Bob");
    // verify second column
    GT_TRUE(df->get_string(1, 0)->equals(val1));
    GT_TRUE(df->get_string(1, 1)->equals(val2));
    GT_TRUE(df->get_string(1, 2)->equals(val3));

    df->print();

    delete maria;
    delete chase;
    delete bob;
    delete val1;
    delete val2;
    delete val3;
    delete first;
    delete second;
    delete third;
    delete df;
    delete schema;
    exit(0);
}

TEST(W1, dataframe_from_schema)
{
    ASSERT_EXIT_ZERO(dataframe_from_schema);
}

void dataframe_from_df()
{
    // Create schema
    Schema *schema = new Schema();
    String *str = new String("First");
    String *str2 = new String("Second");
    String *str3 = new String("One");
    String *str4 = new String("Two");
    // Add two cols
    schema->add_column('I', str);
    schema->add_column('S', str2);
    // Add two rows
    schema->add_row(str3);
    schema->add_row(str4);

    // Create dataframe with above schema 'IS'
    DataFrame *df = new DataFrame(*schema);
    // Create three rows with above schema
    String *maria = new String("Maria");
    Row *first = new Row(*schema);
    first->set(0, 100);
    first->set(1, maria);
    String *chase = new String("Chase");
    Row *second = new Row(*schema);
    second->set(0, 200);
    second->set(1, chase);
    // Add rows to df
    df->add_row(*first);
    df->add_row(*second);

    // Create new dataframe with same structure
    DataFrame *df_copy = new DataFrame(*df);
    Schema scm = df->get_schema();
    GT_EQUALS(df_copy->ncols(), 2);
    GT_EQUALS(df_copy->nrows(), 0);

    // Create rows
    Row *row1 = new Row(scm);
    String *val1 = new String("val1");
    row1->set(0, 5);
    row1->set(1, val1);
    Row *row2 = new Row(scm);
    String *val2 = new String("val2");
    row2->set(0, 6);
    row2->set(1, val2);
    Row *row3 = new Row(scm);
    String *val3 = new String("val3");
    row3->set(0, 7);
    row3->set(1, val3);
    // Add them to df
    df_copy->add_row(*row1);
    df_copy->add_row(*row2);
    df_copy->add_row(*row3);

    // Verify values
    GT_EQUALS(df_copy->nrows(), 3);
    GT_EQUALS(df_copy->get_int(0, 0), 5);
    GT_EQUALS(df_copy->get_int(0, 1), 6);
    GT_EQUALS(df_copy->get_int(0, 2), 7);

    // Create column
    BoolColumn *bools = new BoolColumn(3, true, true, false);
    // Add to df
    String *boolColName = new String("Booleans");
    df_copy->add_column(bools, boolColName);
    // Verify column was added
    GT_EQUALS(df_copy->get_col(*boolColName), 2);
    GT_TRUE(df_copy->get_bool(2, 0));
    GT_TRUE(df_copy->get_bool(2, 1));
    GT_FALSE(df_copy->get_bool(2, 2));

    // Set values in df
    df_copy->set(0, 0, 0);
    df_copy->set(2, 0, false);
    // Verify set
    GT_EQUALS(df_copy->get_int(0, 0), 0);
    GT_FALSE(df_copy->get_bool(2, 0));

    delete maria;
    delete chase;
    delete first;
    delete second;
    delete str;
    delete str2;
    delete str3;
    delete str4;
    delete df_copy;
    delete df;
    delete row1;
    delete val1;
    delete row2;
    delete val2;
    delete row3;
    delete val3;
    delete schema;
    exit(0);
}

TEST(W1, dataframe_from_df)
{
    ASSERT_EXIT_ZERO(dataframe_from_df);
}

void dataframe_filter()
{
    // Create schema with ints
    Schema *schema = new Schema("III");

    // Create df full of ints
    DataFrame *df = new DataFrame(*schema);
    GT_EQUALS(df->ncols(), 3);
    GT_EQUALS(df->nrows(), 0);

    // Create rows
    Row *row1 = new Row(*schema);
    row1->set_idx(0);
    row1->set(0, 10);
    row1->set(1, 2);
    row1->set(2, 6);
    Row *row2 = new Row(*schema);
    row2->set_idx(1);
    row2->set(0, 1);
    row2->set(1, 2);
    row2->set(2, 3);
    Row *row3 = new Row(*schema);
    row3->set_idx(2);
    row3->set(0, 0);
    row3->set(1, 40);
    row3->set(2, 3);

    // Verify row indices
    GT_EQUALS(row1->get_idx(), 0);
    GT_EQUALS(row2->get_idx(), 1);
    GT_EQUALS(row3->get_idx(), 2);

    // Add rows
    df->add_row(*row1);
    df->add_row(*row2);
    df->add_row(*row3);

    //  Create a row that filters out rows whose sum aren't 6
    // schema must be all ints
    FilterSumRower *rower = new FilterSumRower(6);
    DataFrame *filtered = df->filter(*rower);

    // assert values are correct and rows were filtered
    GT_EQUALS(filtered->ncols(), 3);
    GT_EQUALS(filtered->nrows(), 1);
    GT_EQUALS(filtered->get_int(0, 0), 1);
    GT_EQUALS(filtered->get_int(1, 0), 2);
    GT_EQUALS(filtered->get_int(2, 0), 3);

    delete schema;
    delete df;
    delete row1;
    delete row2;
    delete row3;
    delete rower;
    delete filtered;
    exit(0);
}

TEST(W1, dataframe_filter)
{
    ASSERT_EXIT_ZERO(dataframe_filter);
}

void dataframe_map()
{
    // Create schema with ints
    Schema *schema = new Schema("III");

    // Create df full of ints
    DataFrame *df = new DataFrame(*schema);
    GT_EQUALS(df->ncols(), 3);
    GT_EQUALS(df->nrows(), 0);

    // Create rows
    Row *row1 = new Row(*schema);
    row1->set_idx(0);
    row1->set(0, 10);
    row1->set(1, 2);
    row1->set(2, 6);
    Row *row2 = new Row(*schema);
    row2->set_idx(1);
    row2->set(0, 1);
    row2->set(1, 2);
    row2->set(2, 3);
    Row *row3 = new Row(*schema);
    row3->set_idx(2);
    row3->set(0, 0);
    row3->set(1, 40);
    row3->set(2, 3);

    // Verify row indices
    GT_EQUALS(row1->get_idx(), 0);
    GT_EQUALS(row2->get_idx(), 1);
    GT_EQUALS(row3->get_idx(), 2);

    // Add rows
    df->add_row(*row1);
    df->add_row(*row2);
    df->add_row(*row3);

    SumRower *rower = new SumRower();
    df->map(*rower);
    printf("sum %d\n", rower->df_sum);
    GT_EQUALS(rower->df_sum, 67);

    delete schema;
    delete df;
    delete row1;
    delete row2;
    delete row3;
    delete rower;
    exit(0);
}

TEST(W1, dataframe_map)
{
    ASSERT_EXIT_ZERO(dataframe_map);
}

void dataframe_fill_row()
{
    // Create schema with ints
    Schema *schema = new Schema("IBFS");

    // Create df full of ints
    DataFrame *df = new DataFrame(*schema);
    GT_EQUALS(df->ncols(), 4);
    GT_EQUALS(df->nrows(), 0);

    // Create rows
    Row *row1 = new Row(*schema);
    row1->set_idx(0);
    row1->set(0, 10);
    row1->set(1, true);
    row1->set(2, float(5.5));
    String *str = new String("maria");
    row1->set(3, str);
    Row *row2 = new Row(*schema);
    row2->set_idx(1);
    row2->set(0, 3);
    row2->set(1, false);
    row2->set(2, float(7));
    String *str2 = new String("orias");
    row2->set(3, str2);

    // Add rows
    df->add_row(*row1);
    df->add_row(*row2);

    // Create empty row
    Row *copy = new Row(*schema);
    // Fill it
    df->fill_row(1, *copy);

    // Verify values
    GT_EQUALS(copy->get_int(0), 3);
    GT_FALSE(copy->get_bool(1));
    GT_EQUALS(copy->get_float(2), float(7));
    GT_TRUE(copy->get_string(3)->equals(str2));

    delete str;
    delete str2;
    delete schema;
    delete row1;
    delete row2;
    delete df;
    delete copy;
    exit(0);
}

TEST(W1, dataframe_fill_row)
{
    ASSERT_EXIT_ZERO(dataframe_fill_row);
}

void dataframe_gets()
{
    // Create schema with ints
    Schema *schema = new Schema("IBFS");

    // Create df full of ints
    DataFrame *df = new DataFrame(*schema);
    GT_EQUALS(df->ncols(), 4);
    GT_EQUALS(df->nrows(), 0);

    // Create rows
    Row *row1 = new Row(*schema);
    row1->set_idx(0);
    row1->set(0, 10);
    row1->set(1, true);
    row1->set(2, float(5.5));
    String *str = new String("maria");
    row1->set(3, str);
    Row *row2 = new Row(*schema);
    row2->set_idx(1);
    row2->set(0, 3);
    row2->set(1, false);
    row2->set(2, float(7));
    String *str2 = new String("orias");
    row2->set(3, str2);
    Row *row3 = new Row(*schema);
    row3->set_idx(2);
    row3->set(0, -5);
    row3->set(1, true);
    row3->set(2, float(35));
    String *str3 = new String("is");
    row3->set(3, str3);
    Row *row4 = new Row(*schema);
    row4->set_idx(3);
    row4->set(0, 20);
    row4->set(1, false);
    row4->set(2, float(5.5));
    String *str4 = new String("the");
    row4->set(3, str4);
    Row *row5 = new Row(*schema);
    row5->set_idx(4);
    row5->set(0, 1);
    row5->set(1, true);
    row5->set(2, float(0));
    String *str5 = new String("best");
    row5->set(3, str5);

    // Add rows
    df->add_row(*row1);
    df->add_row(*row2);
    df->add_row(*row3);
    df->add_row(*row4);
    df->add_row(*row5);

    // Verify values
    // Column 1
    GT_EQUALS(df->get_int(0, 0), 10);
    GT_EQUALS(df->get_int(0, 1), 3);
    GT_EQUALS(df->get_int(0, 2), -5);
    GT_EQUALS(df->get_int(0, 3), 20);
    GT_EQUALS(df->get_int(0, 4), 1);
    // Column2
    GT_TRUE(df->get_bool(1, 0));
    GT_FALSE(df->get_bool(1, 1));
    GT_TRUE(df->get_bool(1, 2));
    GT_FALSE(df->get_bool(1, 3));
    GT_TRUE(df->get_bool(1, 4));
    // Column3
    GT_EQUALS(df->get_float(2, 0), float(5.5));
    GT_EQUALS(df->get_float(2, 1), float(7));
    GT_EQUALS(df->get_float(2, 2), float(35));
    GT_EQUALS(df->get_float(2, 3), float(5.5));
    GT_EQUALS(df->get_float(2, 4), float(0));
    // Column4
    String *str6 = new String("maria");
    String *str7 = new String("orias");
    String *str8 = new String("is");
    String *str9 = new String("the");
    String *str10 = new String("best");
    GT_TRUE(df->get_string(3, 0)->equals(str6));
    GT_TRUE(df->get_string(3, 1)->equals(str7));
    GT_TRUE(df->get_string(3, 2)->equals(str8));
    GT_TRUE(df->get_string(3, 3)->equals(str9));
    GT_TRUE(df->get_string(3, 4)->equals(str10));

    // Add a duplicate row
    df->add_row(*row5);
    GT_EQUALS(df->nrows(), 6);
    // Check values are the same
    GT_EQUALS(df->get_int(0, 4), 1);
    GT_TRUE(df->get_bool(1, 4));
    GT_EQUALS(df->get_float(2, 4), float(0));
    // Delete objects in row
    delete str5;
    delete row5;
    // df should not be affected,
    GT_TRUE(df->get_string(3, 4)->equals(str10));

    delete str;
    delete str2;
    delete str3;
    delete str4;
    delete str6;
    delete str7;
    delete str8;
    delete str9;
    delete str10;
    delete schema;
    delete row1;
    delete row2;
    delete row3;
    delete row4;
    delete df;
    exit(0);
}

TEST(W1, dataframe_gets)
{
    ASSERT_EXIT_ZERO(dataframe_gets);
}

void dataframe_map2()
{
    // Create schema with ints
    Schema *schema = new Schema("SFFF");

    // Create df full of ints
    DataFrame *df = new DataFrame(*schema);
    GT_EQUALS(df->ncols(), 4);
    GT_EQUALS(df->nrows(), 0);

    // Create rows
    Row *row1 = new Row(*schema);
    String *name1 = new String("maria");
    row1->set_idx(0);
    row1->set(0, name1);
    row1->set(1, float(90));
    row1->set(2, float(95));
    row1->set(3, float(97));
    Row *row2 = new Row(*schema);
    String *name2 = new String("chase");
    row2->set_idx(1);
    row2->set(0, name2);
    row2->set(1, float(86));
    row2->set(2, float(78));
    row2->set(3, float(77));
    Row *row3 = new Row(*schema);
    String *name3 = new String("bob");
    row3->set_idx(2);
    row3->set(0, name3);
    row3->set(1, float(78.7));
    row3->set(2, float(98));
    row3->set(3, float(2));

    // Add rows
    df->add_row(*row1);
    df->add_row(*row2);
    df->add_row(*row3);

    // Verify printed statement on console
    HighestGpaRower *rower = new HighestGpaRower();
    df->map(*rower);
    GT_EQUALS(rower->getHigestAverage(), float(94));

    delete schema;
    delete df;
    delete row1;
    delete row2;
    delete row3;
    delete name1;
    delete name2;
    delete name3;
    delete rower;
    exit(0);
}

TEST(W1, dataframe_map2)
{
    ASSERT_EXIT_ZERO(dataframe_map2);
}
