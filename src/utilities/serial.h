#pragma once
// LANGUAGE: CwC

#include "string.h"
#include "helper.h"

class Serial
{
public:

  /** Extracts the char* from a StrBuff
   * Frees the StrBuff, removes a lot of code duplication */
  static char *extract_char_(StrBuff *str_buff)
  {
    String *str = str_buff->get();
    delete str_buff;
    char *c = str->c_str();
    delete str;
    return c;
  }
};

class Deserialize : public Serial
{
public:

  /** Takes in a serialized Object, derives the Object type based on the first field
   * Once Object type is determined, deserializes the entire Object
   * It is up to the user to properly cast it to the correct type upon return */
  static Object *deserialize(char *to_deserialize)
  {
    StrBuff *object_name = new StrBuff();
    size_t index = 0;
    size_t quotes_seen = 0;

    // Get the object name
    while (to_deserialize[index] != '\0')
    {
      char current = to_deserialize[index];
      // Get the string within the first two quotes
      if (current == '\"')
      {
        quotes_seen++;
        // increment index and continue
        index++;
        continue;
      }
      // break now, we have full Object type
      if (quotes_seen >= 2)
      {
        break;
      }
      // we have seen one quote, add to our Object name
      if (quotes_seen == 1)
      {
        object_name->c(current);
      }
      // increment index and continue
      index++;
    }

    char *char_name = extract_char_(object_name);

    if (char_name == "Dataframe")
    {
      // ...deserialize dataframe
      return nullptr;
    }
    else if (char_name == "Msg")
    {
      // ...deserialize Msg
      return nullptr;
    }
    else {
      // We do not support deserialization for this Object
      return nullptr;
    }
  }
};