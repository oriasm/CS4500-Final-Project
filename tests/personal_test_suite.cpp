#include <gtest/gtest.h>
#include "test_utils.cpp"
#include <stdlib.h>
#include <stdio.h>
#include "array/basic_array.cpp"
#include "dataframe/basic_column.cpp"
#include "dataframe/basic_schema.cpp"
#include "dataframe/basic_row.cpp"
#include "dataframe/basic_dataframe.cpp"
#include "dataframe/basic_example.cpp"
#include "sorer/sorer.cpp"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}