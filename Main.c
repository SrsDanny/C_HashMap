#include <stdio.h>
#include "HashMap.h"
#include "StringToIntMap.h"

int main(int argc, char** argv)
{
  HashMap* map = makeS2IMap();
  int* i = malloc(sizeof(int));
  *i = 42;
  putValue(map, "Foo", i);

  deleteMap(map);
  return 0;
}
