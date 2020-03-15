#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../test_utils.cpp"
#include "../../src/sorer/sorer.h"
#include "../../src/dataframe/dataframe.h"

const char *SM_FILE = "../tests/sorer/basic.sor";
const char *LG_FILE = "../tests/sorer/large.sor";
const char *COMPLEX_FILE = "../tests/sorer/complex.sor";

void small_sorer()
{
    // strings from sor file
    String *hello = new String("hello");
    String *one = new String("1");
    String *hi = new String("hi");
    String *world = new String("world");
    String *maria = new String("maria");
    String *exclamation = new String("\"!\"");
    String *question = new String("\"?\"");
    String *dub_exclam = new String("\"!!\"");

    DataFrame *df = get_dataframe(const_cast<char*>(SM_FILE), 0, 500);
    // Verify first column
    GT_TRUE(df->get_string(0, 0)->equals(hello));
    GT_TRUE(df->get_string(0, 1)->equals(one));
    GT_TRUE(df->get_string(0, 2)->equals(hi));
    // Verify second column
    GT_EQUALS(df->get_int(1, 0), 3);
    GT_EQUALS(df->get_int(1, 1), 2);
    GT_EQUALS(df->get_int(1, 2), 3);
    // Verify third column
    GT_TRUE(df->get_string(2, 0)->equals(world));
    GT_TRUE(df->get_string(2, 1)->equals(hi));
    GT_TRUE(df->get_string(2, 2)->equals(maria));
    // Verify fourth column
    GT_TRUE(df->get_string(3, 0)->equals(exclamation));
    GT_TRUE(df->get_string(3, 1)->equals(question));
    GT_TRUE(df->get_string(3, 2)->equals(dub_exclam));
    // Verify fifth column
    GT_TRUE(df->get_bool(4, 0));
    GT_TRUE(df->get_bool(4, 1));
    GT_FALSE(df->get_bool(4, 2));
    // Verify sixth column
    GT_EQUALS(df->get_float(5, 0), float(5.5));
    GT_EQUALS(df->get_float(5, 1), float(6.6));
    GT_EQUALS(df->get_float(5, 2), float(7));

    delete hello;
    delete one;
    delete hi;
    delete world;
    delete maria;
    delete exclamation;
    delete question;
    delete dub_exclam;
    exit(0);
}

TEST(W1, small_sorer)
{
    ASSERT_EXIT_ZERO(small_sorer);
}

void large_sorer()
{
    DataFrame *df = get_dataframe(const_cast<char*>(LG_FILE), 0, 500);
    // Verify Schema
    GT_EQUALS(df->ncols(), 13);
    Schema schema = df->get_schema();
    GT_EQUALS(schema.col_type(0), 'S');
    GT_EQUALS(schema.col_type(1), 'I');
    GT_EQUALS(schema.col_type(2), 'S');
    GT_EQUALS(schema.col_type(3), 'S');
    GT_EQUALS(schema.col_type(4), 'B');
    GT_EQUALS(schema.col_type(5), 'F');
    GT_EQUALS(schema.col_type(6), 'I');
    GT_EQUALS(schema.col_type(7), 'S');
    GT_EQUALS(schema.col_type(8), 'I');
    GT_EQUALS(schema.col_type(9), 'S');
    GT_EQUALS(schema.col_type(10), 'S');
    GT_EQUALS(schema.col_type(11), 'B');
    GT_EQUALS(schema.col_type(12), 'I');

    // Verify length
    GT_EQUALS(df->nrows(), 500);

    delete df;
    exit(0);
}

TEST(W2, large_sorer)
{
    ASSERT_EXIT_ZERO(large_sorer);
}

void complex_sorer()
{
    DataFrame *df = get_dataframe(const_cast<char*>(COMPLEX_FILE), 0, 500);
    // Verify Schema
    GT_EQUALS(df->ncols(), 16);
    Schema schema = df->get_schema();
    GT_EQUALS(schema.col_type(0), 'B');
    GT_EQUALS(schema.col_type(1), 'I');
    GT_EQUALS(schema.col_type(2), 'F');
    GT_EQUALS(schema.col_type(3), 'S');
    GT_EQUALS(schema.col_type(4), 'B');
    GT_EQUALS(schema.col_type(5), 'I');
    GT_EQUALS(schema.col_type(6), 'F');
    GT_EQUALS(schema.col_type(7), 'S');
    GT_EQUALS(schema.col_type(8), 'B');
    GT_EQUALS(schema.col_type(9), 'I');
    GT_EQUALS(schema.col_type(10), 'F');
    GT_EQUALS(schema.col_type(11), 'S');
    GT_EQUALS(schema.col_type(12), 'B');
    GT_EQUALS(schema.col_type(13), 'B');
    GT_EQUALS(schema.col_type(14), 'F');
    GT_EQUALS(schema.col_type(15), 'S');

    // Verify length
    GT_EQUALS(df->nrows(), 3);

    // Operations
    GT_EQUALS(df->get_int(5, 2), 3);
    df->set(5, 2, 10);
    GT_EQUALS(df->get_int(5, 2), 10);

    delete df;
    exit(0);
}

TEST(W2, complex_sorer)
{
    ASSERT_EXIT_ZERO(complex_sorer);
}