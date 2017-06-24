#include "HashMap.h"
#include <string.h>

static int BIN_CAPACITY = 8;

static void fillBins(Bin* bins, size_t binCount) {
  for (size_t i = 0; i < binCount; i++) {
    bins[i].capacity = BIN_CAPACITY;
    bins[i].size = 0;

    bins[i].data = (MapEntry**) malloc(sizeof(MapEntry*) * BIN_CAPACITY);
    memset(bins[i].data, 0, sizeof(MapEntry*) * BIN_CAPACITY);
  }
}

static void emptyBins(HashMap* map) {
  for (size_t i = 0; i < map->binCount; i++) {
    for (size_t j = 0; j < map->bins[i].size; j++) {
      map->keyDestructor(map->bins[i].data[j]->key);
      map->valueDestructor(map->bins[i].data[j]->value);
    }
    free(map->bins[i].data);
  }
  free(map->bins);
}

HashMap* makeMap(size_t binCount, KeyComparator keyEquals, KeyCopier keyCopier, KeyDestructor keyDestructor, KeyHasher keyHasher, ValueDestructor valueDestructor) {
  HashMap* map = (HashMap*) malloc(sizeof(HashMap));
  map->binCount = binCount;
  map->bins = (Bin*) malloc(sizeof(Bin) * binCount);
  fillBins(map->bins, binCount);

  map->keyEquals = keyEquals;
  map->keyCopier = keyCopier;
  map->keyDestructor = keyDestructor;
  map->keyHasher = keyHasher;
  map->valueDestructor = valueDestructor;

  return map;
}

void deleteMap(HashMap* map) {
  emptyBins(map);
  free(map);
}

static MapEntry* newEntryInBin(Bin* bin) {
  if(bin->size == bin->capacity) {
    bin->capacity = bin->capacity + BIN_CAPACITY;
    MapEntry** temp = (MapEntry**) malloc(sizeof(MapEntry*) * bin->capacity);
    memcpy(temp, bin->data, bin->size);
    memset(temp + bin->size, 0, sizeof(MapEntry*) * BIN_CAPACITY);
    free(bin->data);
    bin->data = temp;
  }

  MapEntry* entry = (MapEntry*) malloc(sizeof(MapEntry));
  memset(entry, 0, sizeof(MapEntry));
  bin->data[bin->size++] = entry;
  return entry;
}

static MapEntry* getMapEntry(HashMap* map, void* key, bool create) {
  size_t keyHash = map->keyHasher(key);
  size_t index = keyHash % map->binCount;

  Bin* bin = &map->bins[index];
  for (size_t i = 0; i < bin->size; i++) {
    if(map->keyEquals(bin->data[i]->key, key)) {
      return bin->data[i];
    }
  }
  if(create) {
    MapEntry* entry = newEntryInBin(bin);
    entry->key = map->keyCopier(key);
  }
  return NULL;
}

void* getValue(HashMap* map, void* key) {
  MapEntry* entry = getMapEntry(map, key, false);
  if(entry) {
    return entry->value;
  }
  return NULL;
}

void putValue(HashMap* map, void* key, void* value) {
  MapEntry* entry = getMapEntry(map, key, true);
  if(entry->value) {
    map->valueDestructor(entry->value);
  }
  entry->value = value;
}
