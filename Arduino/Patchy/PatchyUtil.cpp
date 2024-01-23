#include <Arduino.h>
#include "PatchyUtil.h"

/** djb2Hash Constants **/
const long INITIAL_HASH = 5381;
const long LONG_MAX = 2147483647L; // Maximum value of a long

static long PatchyUtil::hashString(const char *str)
{
  long hash = INITIAL_HASH;
  for (int i = 0; str[i] != '\0'; i++)
  {
    hash = ((hash << 5) + hash) + str[i];
    // Apply a modulo operation to keep the hash within the range of a long
    hash = hash % LONG_MAX;
  }
    return hash;
}
