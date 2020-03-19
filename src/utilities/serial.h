#pragma once
// LANGUAGE: CwC

#include <string.h>
#include "object.h"
#include "string.h"
#include "helper.h"
#include "../dataframe/schema.h"

/** Extracts the char* from a StrBuff
   * Frees the StrBuff, removes a lot of code duplication */
char *extract_char_(StrBuff *str_buff)
{
  String *str = str_buff->get();
  delete str_buff;
  char *c = str->c_str();
  delete str;
  return c;
}

/** Takes in a serialized Object, derives the Object type based on the first field
   * Once Object type is determined, deserializes the entire Object
   * It is up to the user to properly cast it to the correct type upon return */
Object *deserialize(char *to_deserialize)
{
  // get first token
  char *object_name = strtok(to_deserialize, ",");

  if (strcmp(object_name, "{Dataframe") == 0)
  {
    // ...deserialize dataframe
    return nullptr;
  }
  else if (strcmp(object_name, "{Msg") == 0)
  {
    // ...deserialize Msg
    return nullptr;
  }

  // {Schema,schema:IFBS,num_rows:3,num_cols:4}
  else if (strcmp(object_name, "{Schema") == 0)
  {
    // get schema
    char *schema_label = strtok(NULL, ":"); // "schema"
    char *schema_str = strtok(NULL, ",");   // "IFBS"

    // get num rows
    char *rows_label = strtok(NULL, ":");   // "num_rows"
    char *num_rows_str = strtok(NULL, ","); // "3"
    size_t num_rows;
    sscanf(num_rows_str, "%zu", &num_rows);

    // get num cols
    char *cols_label = strtok(NULL, ":");   // "num_cols"
    char *num_cols_str = strtok(NULL, ","); // "4"
    size_t num_cols;
    sscanf(num_cols_str, "%zu", &num_cols);

    // recreate schema
    Schema *sch = new Schema(schema_str);
    return sch;
  }
  else
  {
    // We do not support deserialization for this Object
    return nullptr;
  }
}