#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HASH_TABLE_SIZE 100

typedef struct {
    int x;
    int y;
} Tuple;

typedef struct {
    int* array;
    int size;
    int capacity;
} IntArray;

typedef struct HashEntry {
    Tuple key;
    IntArray value;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    HashEntry* entries[HASH_TABLE_SIZE];  // change as needed
} HashTable;

void initHashTable(HashTable* table);
void insert(HashTable* table, Tuple key, int value);
IntArray* search(HashTable* table, Tuple key);
void deleteEntry(HashTable* table, Tuple key);
void freeHashTable(HashTable* table);

void printHashTable(const HashTable* table);

#endif