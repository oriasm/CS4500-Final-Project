// language: CwC
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include "../utilities/array.h"
#include "rower.h"
#include "row.h"

// A rower that prints the sum of all the values in a DataFrame that contains only floats
class HighestGpaRower : public Rower
{
public:
    FloatArray *df_avgs;
    StringArray *students;

    HighestGpaRower()
    {
        df_avgs = new FloatArray();
        students = new StringArray();
    }

    ~HighestGpaRower()
    {
        // Free student names
        for (size_t index = 0; index < students->length(); index++)
        {
            delete students->get(index);
        }
        // Free list of names and floats
        delete students;
        delete df_avgs;
    }

    // Expecting a row with a 'SFFF' type
    // representing a student and their grades
    bool accept(Row &r)
    {
        // Expecting a row with a 'SFFF' type
        size_t width = r.width();
        assert(r.width() == 4);

        // Get student name
        String *val = r.get_string(0);
        String *student = new String(*val);

        // Calculate avg of grades
        float one = r.get_float(1);
        float two = r.get_float(2);
        float three = r.get_float(3);
        float row_avg = (one + two + three) / 3;
        printf("grade of student %s is %f\n", val->c_str(), row_avg);
        students->push(student);
        df_avgs->push(row_avg);

        return true;
    }

    float getHigestAverage()
    {
        float max = 0;
        size_t found = 0;
        for (size_t index = 0; index < df_avgs->length(); index++)
        {
            // Calc max
            float current = df_avgs->get(index);
            if (current > max)
            {
                max = current;
                found = index;
            }
        }

        char *name = students->get(found)->c_str();
        printf("Top grade was %f from student %s\n", max, name);
        return max;
    }

    Rower *clone()
    {
        return new HighestGpaRower();
    }

    void join_delete(Rower *other)
    {
    }
};