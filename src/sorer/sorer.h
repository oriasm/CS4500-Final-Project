#pragma once
#include <iostream>
#include <cassert>
#include <cstdio>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "../utilities/object.h"
#include "../dataframe/dataframe.h"
#include "../dataframe/schema.h"
#include "../dataframe/column.h"
#include "types.h"
#include "helper.h"

/**
 * Author: Maria Orias
 * A series of helper function to integrate an external team's sorer implementation
 */

// Converts sorer's Types enum to chars 
const char *getType(Types t)
{
    switch (t)
    {
    case Types::BOOL:
        return "B";
        break;
    case Types::INT:
        return "I";
        break;
    case Types::FLOAT:
        return "F";
        break;
    case Types::STRING:
        return "S";
        break;
    default:
        return "";
        break;
    }
}

// Creates a schema object from an Array of the Types enum
Schema *create_schema(TypesArray *types)
{
    Schema *schema = new Schema();
    size_t width = types->len();
    for (size_t i = 0; i < width; i++)
    {
        Types curr = types->get(i);
        char type = *getType(curr);
        schema->add_column(type, new String(""));
    }

    return schema;
}

// gets a bool in the file at the given start and end bytes
bool get_bool(char *file, int start, int end)
{
    // remove empty spaces in front and back
    size_t new_start = triml(file, start, end);
    size_t new_end = trimr(file, start, end);

    // check if it is an empty field
    if (new_start <= new_end)
    {
        int res;
        sscanf(file + new_start, "%d", &res);
        return res;
    }
    else
    {
        return 1;
    }
}

// returns an integer in the given file at the given start and end bytes
int get_int(char *file, int start, int end)
{
    // remove empty spaces in front and back
    size_t new_start = triml(file, start, end);
    size_t new_end = trimr(file, start, end);

    // check if it is an empty field
    if (new_start <= new_end)
    {
        return strtoll(&file[new_start], nullptr, 10);
    }
    else
    {
        return 1;
    }
}

// returns a float in the given file at the given start and end bytes
float get_float(char *file, int start, int end)
{
    // remove empty spaces in front and back
    size_t new_start = triml(file, start, end);
    size_t new_end = trimr(file, start, end);

    // check if it is an empty field
    if (new_start <= new_end)
    {
        return strtold(&file[new_start], nullptr);
    }
    else
    {
        return 1;
    }
}

// returns a string in the given file at the given start and end bytes
String *get_string(char *file, int start, int end)
{

    // remove empty spaces in front and back
    size_t new_start = triml(file, start, end);
    size_t new_end = trimr(file, start, end);

    // check if it is an empty field
    if (new_start <= new_end)
    {
        StrBuff *buffer = new StrBuff();
        for (size_t i = new_start; i <= new_end; ++i)
        {
            char *ptr = new char[2];
            ptr[0] = file[i];
            ptr[1] = '\0';
            buffer->c(ptr);
            delete[] ptr;
        }
        String *str = buffer->get();
        delete buffer;
        return str;
    }
    else
    {
        return new String("");
    }
}

/**
 *  creates and returns a dataframe from the given schema (schema), 
 *  array of value locations (columnar) in the given file(file)
 */
DataFrame *create_dataframe(Schema *schema, FieldArray **columnar, char *file)
{
    size_t columns = schema->width();
    DataFrame *df = new DataFrame(*schema);
    if (columns > 0)
    {
        size_t num_rows = columnar[0]->len();

        for (size_t i = 0; i < num_rows; i++)
        {
            Row *row = new Row(*schema);

            for (size_t j = 0; j < columns; j++)
            {
                FieldArray *column = columnar[j];
                char type = schema->col_type(j);

                switch (type)
                {
                case 'B':
                {
                    int start = column->get_start(i);
                    int end = column->get_end(i);

                    bool val = get_bool(file, start, end);
                    row->set(j, val);
                    break;
                }
                case 'I':
                {
                    int start = column->get_start(i);
                    int end = column->get_end(i);

                    int val = get_int(file, start, end);
                    row->set(j, val);
                    break;
                }
                case 'F':
                {
                    int start = column->get_start(i);
                    int end = column->get_end(i);

                    float val = get_float(file, start, end);
                    row->set(j, val);
                    break;
                }
                case 'S':
                {
                    int start = column->get_start(i);
                    int end = column->get_end(i);

                    String *val = get_string(file, start, end);
                    row->set(j, val);
                    delete val;
                    break;
                }
                default:
                    break;
                }
            }
            df->add_row(*row);
            delete row;
        }
    }

    return df;
}

// creates a dataframe with the fields in a file with the given name, length and offset
DataFrame *get_dataframe(char *filename, size_t from, size_t len)
{
    // Make sure the file exists/can be opened
    int fd = open(filename, O_RDONLY);
    assert(fd != -1);

    // Use stat the get the file size
    struct stat st;
    stat(filename, &st);
    size_t file_size = st.st_size;

    assert(from < file_size);

    // Get the number of page sizes we need
    int pg_size = getpagesize();
    size_t pages = (size_t)(file_size / pg_size) + 1;
    size_t ask = pages * pg_size;

    //Mmap for lazy read
    char *file = (char *)mmap(nullptr, ask, PROT_READ, MAP_PRIVATE, fd, 0);

    // Parse the schema
    TypesArray *schema = parse_schema(file);

    // discard the first line if given from != 0
    size_t end = from + len;
    if (from != 0 && from < file_size)
    {
        while (file[from] != '\n')
        {
            assert(file[from] != EOF);
            ++from;
        }
        ++from;
    }
    // if the file size is smaller than the bytes we are supposed to read,
    // we just read the whole file
    if (end >= file_size)
    {
        end = file_size;
    }
    else
    {
        // else we discard the last line
        while (file[end] != '\n')
        {
            assert(end > from);
            --end;
        }
    }

    // Get the data requested by -from and -len and
    // put them into columnar form
    FieldArray **columnar = make_columnar(file, from, end, schema);

    Schema *sch = create_schema(schema);
    DataFrame *df = create_dataframe(sch, columnar, file);

    munmap(file, ask);
    delete[] columnar;
    delete schema;
    delete sch;
    return df;
}
