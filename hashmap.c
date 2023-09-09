#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  if (key == NULL || map == NULL) {
    return;
  }

  int index = hash(key, map->capacity);

  while (map->buckets[index] != NULL) {
    if (is_equal(map->buckets[index]->key, key)) {
      return;
    }

    index = (index + 1) % map->capacity;
  }

  map->buckets[index] = createPair(strdup(key), value);
  map->size++;
  map->current = index;
}

void enlarge(HashMap *map) {
  enlarge_called = 1; // no borrar (testing purposes)

  Pair **auxBuckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  int count = 0;
  for (int i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL) {
      auxBuckets[count] = map->buckets[i];
      count++;
    }
  }

  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->capacity = map->capacity * 2;
  map->size = 0;
}

HashMap *createMap(long capacity) {

  HashMap *map = (HashMap *)malloc(sizeof(HashMap));

  if (map == NULL) {
    return NULL;
  }

  map->capacity = capacity;
  map->size = 0;
  map->current = -1;

  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));

  return map;
}

void eraseMap(HashMap *map, char *key) {

  Pair *pair = searchMap(map, key);

  if (pair != NULL) {
    pair->key = NULL;
    map->size--;
  }
}

Pair *searchMap(HashMap *map, char *key) {
  int index = hash(key, map->capacity);

  while (map->buckets[index] != NULL) {
    if (is_equal(map->buckets[index]->key, key)) {
      map->current = index;

      return map->buckets[index];
    }

    index = (index + 1) % map->capacity;
  }

  map->current = -1;

  return NULL;
}

Pair *firstMap(HashMap *map) {
  int index = 0;

  while (map->buckets[index] == NULL || map->buckets[index]->key == NULL) {

    index = (index + 1) % map->capacity;
  }

  map->current = index;
  return map->buckets[index];
}

Pair *nextMap(HashMap *map) {

  int index = map->current + 1;

  while (map->buckets[index] == NULL || map->buckets[index]->key == NULL) {
    index = index + 1;

    if (index >= map->size) {
      return NULL;
    }
  }

  map->current = index;
  return map->buckets[index];
}
