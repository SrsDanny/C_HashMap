#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct MapEntry {
  void* key;
  void* value;
} MapEntry;

typedef struct Bin {
  size_t capacity;
  size_t size;
  MapEntry** data;
} Bin;

typedef bool (*KeyComparator)(void* lhs, void* rhs);
typedef void* (*KeyCopier)(void*);
typedef void (*KeyDestructor)(void* key);
typedef size_t (*KeyHasher)(void* value);
typedef void (*ValueDestructor)(void* value);

typedef struct HashMap {
  Bin* bins;
  size_t binCount;
  KeyComparator keyEquals;
  KeyCopier keyCopier;
  KeyDestructor keyDestructor;
  KeyHasher keyHasher;
  ValueDestructor valueDestructor;
} HashMap;

HashMap* makeMap(size_t binCount, KeyComparator keyEquals, KeyCopier keyCopier, KeyDestructor keyDestructor, KeyHasher keyHasher, ValueDestructor valueDestructor);
void deleteMap(HashMap* map);

void* getValue(HashMap* map, void* key);
void putValue(HashMap* map, void* key, void* value);
