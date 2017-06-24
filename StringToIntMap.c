#include "StringToIntMap.h"
#include <string.h>

bool stringComparator(void* lhs, void* rhs) {
  return !strcmp(lhs, rhs);
}

void* stringCopier(void* string) {
  size_t len = strlen(string) + 1;
  void* str = malloc(len);
  strcpy(str, string);
  return str;
}

void stringDeleter(void* str) {
  free(str);
}

size_t stringHasher(void* string) {
  size_t hash = 5381;
  unsigned char* str = string;
  int c;

  while ((c = *str++))
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

void intDeleter(void* i) {
  free(i);
}

HashMap* makeS2IMap() {
  return makeMap(100, stringComparator, stringCopier, stringDeleter, stringHasher, intDeleter);
}
